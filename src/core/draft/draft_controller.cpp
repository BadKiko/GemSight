#include "core/draft/draft_controller.h"

#include "core/advisor/advisor_controller.h"
#include "core/draft/role_predictor.h"
#include "core/heroes/hero_catalog.h"
#include "core/gsi/gsi_server.h"
#include "core/session/match_session_controller.h"

#include <QThread>
#include <QtConcurrent>

namespace gemsight::core {

DraftController::DraftController(QObject* parent)
    : QObject(parent)
{
    m_intelCache.open();
    m_debounce.setInterval(50);
    m_debounce.setSingleShot(true);
    connect(&m_debounce, &QTimer::timeout, this, [this]() {
        emit m_enemies.dataChanged(m_enemies.index(0, 0), m_enemies.index(4, 0));
    });
}

bool DraftController::gsiListening() const
{
    return m_gsi && m_gsi->listening();
}

void DraftController::bindSession(MatchSessionController* session)
{
    if (m_session == session)
        return;
    m_session = session;
    if (!m_session)
        return;

    connect(m_session, &MatchSessionController::gameModePolicyChanged, this, [this]() {
        m_gate.setPolicy(m_session->gameModePolicy());
        refreshPolicyLabel();
        m_enemies.resetSlots();
    });
    connect(m_session, &MatchSessionController::draftHeroPicked, this, &DraftController::onHeroPicked);
    connect(m_session, &MatchSessionController::enemySteamIdResolved, this, &DraftController::onEnemySteam);
    connect(m_session, &MatchSessionController::strategyTimeReached, this, &DraftController::onStrategyTime);
}

void DraftController::bindAdvisor(AdvisorController* advisor)
{
    m_advisor = advisor;
}

void DraftController::bindGsi(GsiServer* gsi)
{
    m_gsi = gsi;
    if (!m_gsi)
        return;

    connect(m_gsi, &GsiServer::listeningChanged, this, &DraftController::gsiListeningChanged);
    connect(m_gsi, &GsiServer::payloadReceived, m_session, &MatchSessionController::ingestPayload);
    connect(m_gsi, &GsiServer::errorOccurred, this, [this](const QString& msg) { setStatus(msg); });
}

void DraftController::startGsi()
{
    if (m_gsi)
        m_gsi->start();
    setStatus(tr("Ожидание GSI на порту 27042…"));
}

void DraftController::simulateTurboDraft()
{
    m_evaluator.reset();
    if (m_session)
        m_session->simulateTurboDraft();
    populateDemoAllies();
    if (m_advisor)
        m_advisor->loadDemoRecommendations();
    m_evaluator.loadDemoThreats();
    setStatus(tr("Демо: Turbo, разведка врагов в фазе банов"));
}

void DraftController::refreshPolicyLabel()
{
    if (!m_session)
        return;
    m_policyLabel = gameModePolicyToString(m_session->gameModePolicy());
    emit policyLabelChanged();
}

void DraftController::setStatus(const QString& line)
{
    if (m_statusLine == line)
        return;
    m_statusLine = line;
    emit statusLineChanged();
}

void DraftController::onHeroPicked(int teamSlot, int heroId, const QString& heroName)
{
    if (teamSlot < 0 || teamSlot >= 5)
        return;

    gemsight::EnemySlot slot;
    slot.teamSlot = teamSlot;
    slot.heroId = heroId;
    slot.heroName = heroName;
    slot.statusText = tr("Герой выбран");
    decorateHeroVisuals(slot, heroId);
    applyRoleGuess(slot);
    m_enemies.updateSlot(teamSlot, slot);
    refreshEvaluatorFromEnemies();
    m_debounce.start();
}

void DraftController::onEnemySteam(int teamSlot, qint64 steamId, const QString& name)
{
    if (teamSlot < 0 || teamSlot >= 5)
        return;

    gemsight::EnemySlot slot;
    const QModelIndex idx = m_enemies.index(teamSlot);
    slot.heroId = m_enemies.data(idx, gemsight::EnemyTeamModel::HeroIdRole).toInt();
    slot.heroName = m_enemies.data(idx, gemsight::EnemyTeamModel::HeroNameRole).toString();
    slot.teamSlot = teamSlot;
    slot.steamId = steamId;

    const bool allowed = m_gate.mayFetchEnemyProfile(steamId, teamSlot);
    slot.profileUnlocked = allowed;
    if (allowed) {
        slot.playerName = name.isEmpty() ? QStringLiteral("ID %1").arg(steamId) : name;
        slot.statusText = tr("Профиль STRATZ…");
        scheduleStratzStub(teamSlot, steamId);
    } else {
        slot.playerName = tr("Скрыто до Strategy Time");
        slot.statusText = tr("Только мета героя");
    }
    m_enemies.updateSlot(teamSlot, slot);
}

void DraftController::onStrategyTime()
{
    m_gate.setStrategyTimeReached(true);
    setStatus(tr("Strategy Time — разблокировка профилей врагов"));
    for (int i = 0; i < 5; ++i) {
        const QModelIndex idx = m_enemies.index(i);
        const qint64 steamId = m_enemies.data(idx, gemsight::EnemyTeamModel::SteamIdRole).toLongLong();
        if (steamId > 0 && m_gate.mayFetchEnemyProfile(steamId, i))
            scheduleStratzStub(i, steamId);
    }
}

void DraftController::scheduleStratzStub(int teamSlot, qint64 steamId)
{
    const QString cachedAvatar = m_intelCache.avatarUrl(steamId, m_activePatch);
    if (!cachedAvatar.isEmpty()) {
        QMetaObject::invokeMethod(this, [this, teamSlot, steamId, cachedAvatar]() {
            gemsight::EnemySlot slot;
            const QModelIndex idx = m_enemies.index(teamSlot);
            slot.teamSlot = teamSlot;
            slot.steamId = steamId;
            slot.heroId = m_enemies.data(idx, gemsight::EnemyTeamModel::HeroIdRole).toInt();
            slot.heroName = m_enemies.data(idx, gemsight::EnemyTeamModel::HeroNameRole).toString();
            slot.heroPortraitUrl = m_enemies.data(idx, gemsight::EnemyTeamModel::HeroPortraitUrlRole).toString();
            slot.playerName = m_enemies.data(idx, gemsight::EnemyTeamModel::PlayerNameRole).toString();
            slot.avatarUrl = cachedAvatar;
            slot.profileUnlocked = true;
            slot.winRate = 0.52 + (teamSlot * 0.01);
            slot.statusText = tr("Кэш SQLite");
            applyRoleGuess(slot);
            m_enemies.updateSlot(teamSlot, slot);
        }, Qt::QueuedConnection);
        return;
    }

    QtConcurrent::run([this, teamSlot, steamId]() {
        QThread::msleep(250);

        QString avatarUrl;
        QString displayName;
        const PlayerIntelResult intel = m_intelFetcher.fetchAvatarOpenDota(steamId);
        if (intel.ok) {
            avatarUrl = intel.avatarUrl;
            displayName = intel.displayName;
        }

        QMetaObject::invokeMethod(
            this,
            [this, teamSlot, steamId, avatarUrl, displayName]() {
                if (!avatarUrl.isEmpty())
                    m_intelCache.putPlayerProfile(steamId, m_activePatch, avatarUrl, QString());
                gemsight::EnemySlot slot;
                const QModelIndex idx = m_enemies.index(teamSlot);
                slot.teamSlot = teamSlot;
                slot.steamId = steamId;
                slot.heroId = m_enemies.data(idx, gemsight::EnemyTeamModel::HeroIdRole).toInt();
                slot.heroName = m_enemies.data(idx, gemsight::EnemyTeamModel::HeroNameRole).toString();
                slot.heroPortraitUrl = m_enemies.data(idx, gemsight::EnemyTeamModel::HeroPortraitUrlRole).toString();
                slot.playerName = m_enemies.data(idx, gemsight::EnemyTeamModel::PlayerNameRole).toString();
                if (!displayName.isEmpty())
                    slot.playerName = displayName;
                slot.avatarUrl = avatarUrl;
                slot.profileUnlocked = true;
                slot.winRate = 0.52 + (teamSlot * 0.01);
                slot.statusText = avatarUrl.isEmpty() ? tr("Профиль (без аватара)") : tr("OpenDota / кэш");
                applyRoleGuess(slot);
                m_enemies.updateSlot(teamSlot, slot);
            },
            Qt::QueuedConnection);
    });
}

void DraftController::decorateHeroVisuals(gemsight::EnemySlot& slot, int heroId)
{
    if (heroId <= 0)
        return;
    slot.heroId = heroId;
    slot.heroName = HeroCatalog::displayName(heroId);
    slot.heroPortraitUrl = HeroCatalog::portraitUrl(heroId);
}

void DraftController::applyRoleGuess(gemsight::EnemySlot& slot)
{
    const RoleGuess guess = slot.heroId > 0 ? RolePredictor::guessForHero(slot.heroId, slot.teamSlot)
                                            : RolePredictor::guessForPickOrder(slot.teamSlot);
    slot.roleLabel = guess.label;
    slot.roleConfidence = guess.confidence;
}

void DraftController::refreshEvaluatorFromEnemies()
{
    int revealed = 0;
    for (int i = 0; i < 5; ++i) {
        const QModelIndex idx = m_enemies.index(i);
        if (m_enemies.data(idx, gemsight::EnemyTeamModel::HeroIdRole).toInt() > 0)
            ++revealed;
    }
    m_evaluator.onEnemyPicksRevealed(revealed);
}

void DraftController::populateDemoAllies()
{
    const QList<int> heroIds = {14, 5, 8, 86, 26};
    for (int i = 0; i < 5; ++i) {
        gemsight::EnemySlot slot;
        slot.teamSlot = i;
        decorateHeroVisuals(slot, heroIds.at(i));
        slot.playerName = tr("Союзник %1").arg(i + 1);
        slot.profileUnlocked = true;
        slot.statusText = tr("Пик союзника");
        applyRoleGuess(slot);
        m_allies.updateSlot(i, slot);
    }
}

} // namespace gemsight::core

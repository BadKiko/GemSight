#include "core/draft/draft_controller.h"

#include "core/advisor/advisor_controller.h"
#include "core/draft/role_predictor.h"
#include "core/heroes/hero_catalog.h"
#include "core/gsi/gsi_server.h"
#include "core/session/match_session_controller.h"
#include "core/settings/settings_store.h"

#include <QJsonDocument>
#include <QThread>
#include <QtConcurrent>

namespace gemsight::core {

namespace {

void applySignatureHero(gemsight::EnemySlot& slot, const QVector<StratzHeroPoolEntry>& heroes)
{
    if (heroes.isEmpty())
        return;

    StratzHeroPoolEntry best = heroes.first();
    for (const StratzHeroPoolEntry& entry : heroes) {
        if (entry.matchCount > best.matchCount)
            best = entry;
    }
    slot.signatureHeroId = best.heroId;
    slot.signatureHeroPortraitUrl = HeroCatalog::portraitUrl(best.heroId);
}

} // namespace

DraftController::DraftController(QObject* parent)
    : QObject(parent)
{
    m_intelCache.open();
    m_metaMatrices.attachCache(&m_intelCache);
    m_metaMatrices.ensureSeeded();

    m_debounce.setInterval(50);
    m_debounce.setSingleShot(true);
    connect(&m_debounce, &QTimer::timeout, this, [this]() {
        emit m_enemies.dataChanged(m_enemies.index(0, 0), m_enemies.index(4, 0));
        if (!m_demoAdvisor)
            refreshAdvisor();
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

void DraftController::bindSettings(SettingsStore* settings)
{
    m_settings = settings;
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
    m_demoAdvisor = true;
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
    Q_UNUSED(heroName);
    if (teamSlot < 0 || teamSlot >= 5)
        return;

    m_demoAdvisor = false;

    gemsight::EnemySlot slot;
    slot.teamSlot = teamSlot;
    slot.heroId = heroId;
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
    slot.heroPortraitUrl = m_enemies.data(idx, gemsight::EnemyTeamModel::HeroPortraitUrlRole).toString();
    slot.teamSlot = teamSlot;
    slot.steamId = steamId;

    const bool allowed = m_gate.mayFetchEnemyProfile(steamId, teamSlot);
    slot.profileUnlocked = allowed;
    if (allowed) {
        slot.playerName = name.isEmpty() ? QStringLiteral("ID %1").arg(steamId) : name;
        slot.statusText = tr("Профиль STRATZ…");
        schedulePlayerIntel(teamSlot, steamId);
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
            schedulePlayerIntel(i, steamId);
    }
}

void DraftController::applyIntelToSlot(
    int teamSlot,
    qint64 steamId,
    const StratzPlayerIntel& stratz,
    const PlayerIntelResult& openDota)
{
    gemsight::EnemySlot slot;
    const QModelIndex idx = m_enemies.index(teamSlot);
    slot.teamSlot = teamSlot;
    slot.steamId = steamId;
    slot.heroId = m_enemies.data(idx, gemsight::EnemyTeamModel::HeroIdRole).toInt();
    slot.heroName = m_enemies.data(idx, gemsight::EnemyTeamModel::HeroNameRole).toString();
    slot.heroPortraitUrl = m_enemies.data(idx, gemsight::EnemyTeamModel::HeroPortraitUrlRole).toString();
    slot.playerName = m_enemies.data(idx, gemsight::EnemyTeamModel::PlayerNameRole).toString();

    if (stratz.ok && !stratz.displayName.isEmpty())
        slot.playerName = stratz.displayName;
    else if (openDota.ok && !openDota.displayName.isEmpty())
        slot.playerName = openDota.displayName;

    if (!stratz.avatarUrl.isEmpty())
        slot.avatarUrl = stratz.avatarUrl;
    else if (openDota.ok)
        slot.avatarUrl = openDota.avatarUrl;

    if (stratz.winRate >= 0.0)
        slot.winRate = stratz.winRate;
    else
        slot.winRate = 0.52 + (teamSlot * 0.01);

    applySignatureHero(slot, stratz.heroes);
    slot.profileUnlocked = true;

    if (stratz.ok)
        slot.statusText = tr("STRATZ");
    else if (!slot.avatarUrl.isEmpty())
        slot.statusText = tr("OpenDota / кэш");
    else
        slot.statusText = tr("Профиль (без аватара)");

    applyRoleGuess(slot);
    m_enemies.updateSlot(teamSlot, slot);
}

void DraftController::schedulePlayerIntel(int teamSlot, qint64 steamId)
{
    const QString cachedAvatar = m_intelCache.avatarUrl(steamId, m_activePatch);
    const QString cachedPayload = m_intelCache.playerPayload(steamId, m_activePatch);
    const QString token = m_settings ? m_settings->stratzApiToken() : QString();

    if (!cachedAvatar.isEmpty() && !cachedPayload.isEmpty()) {
        StratzPlayerIntel parsed;
        parsed.steamId = steamId;
        const QJsonDocument doc = QJsonDocument::fromJson(cachedPayload.toUtf8());
        if (doc.isObject()) {
            StratzPlayerIntelService parser;
            parsed = parser.parsePlayerData(doc.object(), steamId);
        }
        parsed.avatarUrl = cachedAvatar;
        parsed.ok = true;
        QMetaObject::invokeMethod(this, [this, teamSlot, steamId, parsed]() { applyIntelToSlot(teamSlot, steamId, parsed, {}); }, Qt::QueuedConnection);
        return;
    }

    QtConcurrent::run([this, teamSlot, steamId, token]() {
        StratzPlayerIntel stratz;
        if (!token.isEmpty())
            stratz = m_stratzService.fetchPlayer(token, steamId);

        PlayerIntelResult openDota;
        if (!stratz.ok || stratz.avatarUrl.isEmpty())
            openDota = m_intelFetcher.fetchAvatarOpenDota(steamId);

        QMetaObject::invokeMethod(
            this,
            [this, teamSlot, steamId, stratz, openDota]() {
                const QString avatar = !stratz.avatarUrl.isEmpty() ? stratz.avatarUrl : openDota.avatarUrl;
                const QString payload = stratz.rawJson;
                if (!avatar.isEmpty() || !payload.isEmpty())
                    m_intelCache.putPlayerProfile(steamId, m_activePatch, avatar, payload);
                applyIntelToSlot(teamSlot, steamId, stratz, openDota);
                if (!m_demoAdvisor)
                    refreshAdvisor();
            },
            Qt::QueuedConnection);
    });
}

DraftSnapshot DraftController::buildSnapshot() const
{
    DraftSnapshot snap;
    for (int i = 0; i < 5; ++i) {
        const QModelIndex idx = m_enemies.index(i);
        snap.enemyHeroIds.push_back(m_enemies.data(idx, gemsight::EnemyTeamModel::HeroIdRole).toInt());
        snap.enemySteamIds.push_back(m_enemies.data(idx, gemsight::EnemyTeamModel::SteamIdRole).toLongLong());
    }
    for (int i = 0; i < 5; ++i) {
        const QModelIndex idx = m_allies.index(i);
        snap.allyHeroIds.push_back(m_allies.data(idx, gemsight::EnemyTeamModel::HeroIdRole).toInt());
    }
    return snap;
}

void DraftController::refreshAdvisor()
{
    if (!m_advisor || m_demoAdvisor)
        return;
    m_advisor->evaluateFromDraft(buildSnapshot(), &m_metaMatrices);
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

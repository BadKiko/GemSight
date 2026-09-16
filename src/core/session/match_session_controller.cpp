#include "core/session/match_session_controller.h"

#include <QJsonArray>
#include <QJsonObject>

namespace gemsight::core {

namespace {
QString heroIdToPlaceholderName(int heroId)
{
    if (heroId <= 0)
        return QString();
    return QStringLiteral("hero_%1").arg(heroId);
}
} // namespace

MatchSessionController::MatchSessionController(QObject* parent)
    : QObject(parent)
{
}

QString MatchSessionController::scoutPolicy() const
{
    return gameModePolicyToString(m_policy);
}

void MatchSessionController::setPhase(const QString& phase)
{
    if (m_matchPhase == phase)
        return;
    m_matchPhase = phase;
    emit matchPhaseChanged();

    if (phase.contains(QStringLiteral("STRATEGY"), Qt::CaseInsensitive) && !m_strategyEmitted) {
        m_strategyEmitted = true;
        emit strategyTimeReached();
    }
}

void MatchSessionController::ingestPayload(const QJsonObject& payload)
{
    const QJsonObject map = payload.value(QStringLiteral("map")).toObject();
    if (!map.isEmpty())
        parseMap(map);

    const QJsonObject draft = payload.value(QStringLiteral("draft")).toObject();
    if (!draft.isEmpty())
        parseDraft(draft);

    const QJsonObject player = payload.value(QStringLiteral("player")).toObject();
    if (!player.isEmpty()) {
        const QString team = player.value(QStringLiteral("team_name")).toString();
        Q_UNUSED(team);
    }

    if (payload.contains(QStringLiteral("roster"))) {
        const QJsonValue rosterVal = payload.value(QStringLiteral("roster"));
        QJsonArray players;
        if (rosterVal.isArray())
            players = rosterVal.toArray();
        else if (rosterVal.isObject()) {
            const QString raw = rosterVal.toObject().value(QStringLiteral("players")).toString();
            const QJsonDocument doc = QJsonDocument::fromJson(raw.toUtf8());
            if (doc.isArray())
                players = doc.array();
        }
        for (const QJsonValue& v : players) {
            if (!v.isObject())
                continue;
            parseRosterPlayer(v.toObject(), true);
        }
    }
}

void MatchSessionController::parseMap(const QJsonObject& map)
{
    const QString state = map.value(QStringLiteral("game_state")).toString();
    if (!state.isEmpty())
        setPhase(state);

    const QString mode = map.value(QStringLiteral("game_mode")).toString();
    const QString lobby = map.value(QStringLiteral("lobby_type")).toString();
    if (!mode.isEmpty() || !lobby.isEmpty()) {
        m_gameModeLabel = mode.isEmpty() ? lobby : mode;
        m_policy = gameModePolicyFromLobby(mode, lobby);
        emit gameModeLabelChanged();
        emit gameModePolicyChanged();
    }
}

void MatchSessionController::parseDraft(const QJsonObject& draft)
{
    setPhase(QStringLiteral("DOTA_GAMERULES_STATE_HERO_SELECTION"));
    for (int team = 2; team <= 3; ++team) {
        const bool isDire = team == 3;
        if (!isDire)
            continue;
        for (int pick = 0; pick < 5; ++pick) {
            const QString key = QStringLiteral("team%1:pick%2_id").arg(team).arg(pick);
            const int heroId = draft.value(key).toInt();
            if (heroId > 0)
                emit draftHeroPicked(pick, heroId, heroIdToPlaceholderName(heroId));
        }
    }
}

void MatchSessionController::parseRosterPlayer(const QJsonObject& player, bool isEnemy)
{
    Q_UNUSED(isEnemy);
    const int team = player.value(QStringLiteral("team")).toInt();
    if (team != 3)
        return;

    int slot = player.value(QStringLiteral("team_slot")).toInt(-1);
    if (slot < 0)
        slot = player.value(QStringLiteral("index")).toInt();

    const QString steamStr = player.value(QStringLiteral("steamId")).toString().trimmed();
    qint64 steamId = steamStr.toLongLong();
    const QString name = player.value(QStringLiteral("name")).toString();
    const QString hero = player.value(QStringLiteral("hero")).toString();

    if (!hero.isEmpty())
        emit draftHeroPicked(slot, 0, hero);

    if (steamId > 0 && slot >= 0 && slot < 5)
        emit enemySteamIdResolved(slot, steamId, name);
}

void MatchSessionController::simulateTurboDraft()
{
    m_policy = GameModePolicy::ScoutEarly;
    m_gameModeLabel = QStringLiteral("GameMode_Turbo");
    emit gameModeLabelChanged();
    emit gameModePolicyChanged();
    setPhase(QStringLiteral("DOTA_GAMERULES_STATE_HERO_SELECTION"));

    const QList<int> heroes = {14, 1, 2, 3, 4};
    for (int i = 0; i < heroes.size(); ++i)
        emit draftHeroPicked(i, heroes.at(i), heroIdToPlaceholderName(heroes.at(i)));

    const QList<qint64> ids = {101270074, 101270075, 101270076, 101270077, 101270078};
    for (int i = 0; i < ids.size(); ++i)
        emit enemySteamIdResolved(i, ids.at(i), QStringLiteral("Игрок %1").arg(i + 1));
}

} // namespace gemsight::core

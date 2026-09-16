#include "core/draft/game_mode_policy.h"

namespace gemsight::core {

QString gameModePolicyToString(GameModePolicy policy)
{
    switch (policy) {
    case GameModePolicy::ScoutEarly:
        return QStringLiteral("ScoutEarly");
    case GameModePolicy::ScoutRanked:
        return QStringLiteral("ScoutRanked");
    case GameModePolicy::ScoutCaptains:
        return QStringLiteral("ScoutCaptains");
    default:
        return QStringLiteral("Unknown");
    }
}

GameModePolicy gameModePolicyFromLobby(const QString& gameMode, const QString& lobbyType)
{
    const QString mode = gameMode.toLower();
    const QString lobby = lobbyType.toLower();

    if (mode.contains(QStringLiteral("captain")) || mode.contains(QStringLiteral("cm"))
        || mode.contains(QStringLiteral("cd")))
        return GameModePolicy::ScoutCaptains;

    if (mode.contains(QStringLiteral("turbo")) || lobby.contains(QStringLiteral("unranked"))
        || mode.contains(QStringLiteral("all_pick")) && !mode.contains(QStringLiteral("ranked")))
        return GameModePolicy::ScoutEarly;

    if (mode.contains(QStringLiteral("ranked")) || lobby.contains(QStringLiteral("ranked")))
        return GameModePolicy::ScoutRanked;

    if (mode.contains(QStringLiteral("turbo")))
        return GameModePolicy::ScoutEarly;

    return GameModePolicy::ScoutEarly;
}

} // namespace gemsight::core

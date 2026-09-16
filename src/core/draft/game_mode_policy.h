#pragma once

#include <QString>

namespace gemsight::core {

enum class GameModePolicy {
    Unknown,
    ScoutEarly,
    ScoutRanked,
    ScoutCaptains,
};

QString gameModePolicyToString(GameModePolicy policy);
GameModePolicy gameModePolicyFromLobby(const QString& gameMode, const QString& lobbyType);

} // namespace gemsight::core

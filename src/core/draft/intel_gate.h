#pragma once

#include "core/draft/game_mode_policy.h"

#include <QtGlobal>

namespace gemsight::core {

class IntelGate {
public:
    explicit IntelGate(GameModePolicy policy = GameModePolicy::Unknown);

    void setPolicy(GameModePolicy policy);
    GameModePolicy policy() const { return m_policy; }

    void setStrategyTimeReached(bool reached);
    void setSlotUnlocked(int teamSlot, bool unlocked);

    bool mayFetchEnemyProfile(qint64 steamAccountId, int teamSlot) const;
    bool mayFetchTeammateProfile() const { return true; }

private:
    GameModePolicy m_policy = GameModePolicy::Unknown;
    bool m_strategyTime = false;
    bool m_slotUnlocked[5] = {};
};

} // namespace gemsight::core

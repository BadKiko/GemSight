#include "core/draft/intel_gate.h"

namespace gemsight::core {

IntelGate::IntelGate(GameModePolicy policy)
    : m_policy(policy)
{
}

void IntelGate::setPolicy(GameModePolicy policy)
{
    m_policy = policy;
}

void IntelGate::setStrategyTimeReached(bool reached)
{
    m_strategyTime = reached;
}

void IntelGate::setSlotUnlocked(int teamSlot, bool unlocked)
{
    if (teamSlot >= 0 && teamSlot < 5)
        m_slotUnlocked[teamSlot] = unlocked;
}

bool IntelGate::mayFetchEnemyProfile(qint64 steamAccountId, int teamSlot) const
{
    if (steamAccountId <= 0)
        return false;

    switch (m_policy) {
    case GameModePolicy::ScoutEarly:
        return true;
    case GameModePolicy::ScoutRanked:
    case GameModePolicy::ScoutCaptains:
        return m_strategyTime || (teamSlot >= 0 && teamSlot < 5 && m_slotUnlocked[teamSlot]);
    default:
        return m_strategyTime;
    }
}

} // namespace gemsight::core

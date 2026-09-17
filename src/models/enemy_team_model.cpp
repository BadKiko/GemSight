#include "models/enemy_team_model.h"

namespace gemsight {

EnemyTeamModel::EnemyTeamModel(QObject* parent)
    : QAbstractListModel(parent)
{
    resetSlots();
}

int EnemyTeamModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;
    return m_slots.size();
}

QVariant EnemyTeamModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_slots.size())
        return {};

    const EnemySlot& slot = m_slots.at(index.row());
    switch (role) {
    case TeamSlotRole:
        return slot.teamSlot;
    case HeroIdRole:
        return slot.heroId;
    case HeroNameRole:
        return slot.heroName;
    case PlayerNameRole:
        return slot.playerName;
    case StatusTextRole:
        return slot.statusText;
    case WinRateRole:
        return slot.winRate;
    case ProfileUnlockedRole:
        return slot.profileUnlocked;
    case SteamIdRole:
        return QVariant::fromValue(slot.steamId);
    case RoleLabelRole:
        return slot.roleLabel;
    case RoleConfidenceRole:
        return slot.roleConfidence;
    case AvatarUrlRole:
        return slot.avatarUrl;
    case HeroPortraitUrlRole:
        return slot.heroPortraitUrl;
    case SignatureHeroIdRole:
        return slot.signatureHeroId;
    case SignatureHeroPortraitUrlRole:
        return slot.signatureHeroPortraitUrl;
    default:
        return {};
    }
}

QHash<int, QByteArray> EnemyTeamModel::roleNames() const
{
    return {
        {TeamSlotRole, "teamSlot"},
        {HeroIdRole, "heroId"},
        {HeroNameRole, "heroName"},
        {PlayerNameRole, "playerName"},
        {StatusTextRole, "statusText"},
        {WinRateRole, "winRate"},
        {ProfileUnlockedRole, "profileUnlocked"},
        {SteamIdRole, "steamId"},
        {RoleLabelRole, "roleLabel"},
        {RoleConfidenceRole, "roleConfidence"},
        {AvatarUrlRole, "avatarUrl"},
        {HeroPortraitUrlRole, "heroPortraitUrl"},
        {SignatureHeroIdRole, "signatureHeroId"},
        {SignatureHeroPortraitUrlRole, "signatureHeroPortraitUrl"},
    };
}

void EnemyTeamModel::resetSlots()
{
    beginResetModel();
    m_slots.clear();
    for (int i = 0; i < 5; ++i) {
        EnemySlot s;
        s.teamSlot = i;
        s.statusText = QStringLiteral("—");
        m_slots.push_back(s);
    }
    endResetModel();
}

void EnemyTeamModel::updateSlot(int teamSlot, const EnemySlot& slot)
{
    if (teamSlot < 0 || teamSlot >= m_slots.size())
        return;
    m_slots[teamSlot] = slot;
    m_slots[teamSlot].teamSlot = teamSlot;
    const QModelIndex idx = index(teamSlot);
    emit dataChanged(idx, idx);
}

} // namespace gemsight

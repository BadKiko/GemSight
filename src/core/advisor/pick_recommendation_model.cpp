#include "core/advisor/pick_recommendation_model.h"

namespace gemsight::core {

PickRecommendationModel::PickRecommendationModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

int PickRecommendationModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;
    return m_rows.size();
}

QVariant PickRecommendationModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_rows.size())
        return {};

    const PickRecommendationRow& row = m_rows.at(index.row());
    switch (role) {
    case HeroIdRole:
        return row.heroId;
    case HeroNameRole:
        return row.heroName;
    case ScoreRole:
        return row.score;
    case TierRole:
        return static_cast<int>(row.tier);
    case MessageRole:
        return row.message;
    case MatchupAdvantageRole:
        return row.matchupAdvantage;
    case PersonalMasteryRole:
        return row.personalMastery;
    case GamesTotalRole:
        return row.gamesTotal;
    case Games30dRole:
        return row.games30d;
    case IsSignatureRole:
        return row.isSignature;
    case HeroPortraitUrlRole:
        return row.heroPortraitUrl;
    default:
        return {};
    }
}

QHash<int, QByteArray> PickRecommendationModel::roleNames() const
{
    return {
        {HeroIdRole, "heroId"},
        {HeroNameRole, "heroName"},
        {ScoreRole, "score"},
        {TierRole, "tier"},
        {MessageRole, "message"},
        {MatchupAdvantageRole, "matchupAdvantage"},
        {PersonalMasteryRole, "personalMastery"},
        {GamesTotalRole, "gamesTotal"},
        {Games30dRole, "games30d"},
        {IsSignatureRole, "isSignature"},
        {HeroPortraitUrlRole, "heroPortraitUrl"},
    };
}

void PickRecommendationModel::resetRows(const QVector<PickRecommendationRow>& rows)
{
    beginResetModel();
    m_rows = rows;
    endResetModel();
}

} // namespace gemsight::core

#include "models/lookahead_threat_model.h"

namespace gemsight {

LookaheadThreatModel::LookaheadThreatModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

int LookaheadThreatModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;
    return m_rows.size();
}

QVariant LookaheadThreatModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_rows.size())
        return {};

    const LookaheadThreatRow& row = m_rows.at(index.row());
    switch (role) {
    case HeroIdRole:
        return row.heroId;
    case HeroNameRole:
        return row.heroName;
    case HeroPortraitUrlRole:
        return row.heroPortraitUrl;
    case ReasonRole:
        return row.reason;
    case ThreatScoreRole:
        return row.threatScore;
    default:
        return {};
    }
}

QHash<int, QByteArray> LookaheadThreatModel::roleNames() const
{
    return {
        {HeroIdRole, "heroId"},
        {HeroNameRole, "heroName"},
        {HeroPortraitUrlRole, "heroPortraitUrl"},
        {ReasonRole, "reason"},
        {ThreatScoreRole, "threatScore"},
    };
}

void LookaheadThreatModel::resetRows(const QVector<LookaheadThreatRow>& rows)
{
    beginResetModel();
    m_rows = rows;
    endResetModel();
}

} // namespace gemsight

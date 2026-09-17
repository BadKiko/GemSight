#pragma once

#include "core/advisor/advisor_types.h"

#include <QAbstractListModel>

namespace gemsight::core {

struct PickRecommendationRow {
    int heroId = 0;
    QString heroName;
    double score = 0.0;
    PickTier tier = PickTier::Neutral;
    QString message;
    double matchupAdvantage = 0.0;
    double personalMastery = 0.0;
    int gamesTotal = 0;
    int games30d = 0;
    bool isSignature = false;
};

class PickRecommendationModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum Roles {
        HeroIdRole = Qt::UserRole + 1,
        HeroNameRole,
        ScoreRole,
        TierRole,
        MessageRole,
        MatchupAdvantageRole,
        PersonalMasteryRole,
        GamesTotalRole,
        Games30dRole,
        IsSignatureRole,
    };
    Q_ENUM(Roles)

    explicit PickRecommendationModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void resetRows(const QVector<PickRecommendationRow>& rows);

private:
    QVector<PickRecommendationRow> m_rows;
};

} // namespace gemsight::core

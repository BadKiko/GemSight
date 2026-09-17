#pragma once

#include <QAbstractListModel>

namespace gemsight {

struct LookaheadThreatRow {
    int heroId = 0;
    QString heroName;
    QString heroPortraitUrl;
    QString reason;
    double threatScore = 0.0;
};

class LookaheadThreatModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum Roles {
        HeroIdRole = Qt::UserRole + 1,
        HeroNameRole,
        HeroPortraitUrlRole,
        ReasonRole,
        ThreatScoreRole,
    };
    Q_ENUM(Roles)

    explicit LookaheadThreatModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void resetRows(const QVector<LookaheadThreatRow>& rows);

private:
    QVector<LookaheadThreatRow> m_rows;
};

} // namespace gemsight

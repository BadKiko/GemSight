#pragma once

#include <QAbstractListModel>

namespace gemsight {

struct EnemySlot {
    int teamSlot = 0;
    int heroId = 0;
    QString heroName;
    QString playerName;
    QString statusText;
    double winRate = -1.0;
    bool profileUnlocked = false;
    qint64 steamId = 0;
    QString roleLabel;
    double roleConfidence = 0.0;
};

class EnemyTeamModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum Roles {
        TeamSlotRole = Qt::UserRole + 1,
        HeroIdRole,
        HeroNameRole,
        PlayerNameRole,
        StatusTextRole,
        WinRateRole,
        ProfileUnlockedRole,
        SteamIdRole,
        RoleLabelRole,
        RoleConfidenceRole,
    };
    Q_ENUM(Roles)

    explicit EnemyTeamModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void resetSlots();
    void updateSlot(int teamSlot, const EnemySlot& slot);

private:
    QVector<EnemySlot> m_slots;
};

} // namespace gemsight

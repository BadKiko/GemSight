#pragma once

#include <QObject>
#include <QString>

namespace gemsight::core {

struct PlayerIntelResult {
    qint64 steamId = 0;
    QString avatarUrl;
    QString displayName;
    bool ok = false;
};

class PlayerIntelFetcher : public QObject {
    Q_OBJECT
public:
    explicit PlayerIntelFetcher(QObject* parent = nullptr);

    PlayerIntelResult fetchAvatarOpenDota(qint64 steamId) const;
};

} // namespace gemsight::core

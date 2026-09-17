#pragma once

#include <QString>
#include <QVector>

namespace gemsight::core {

struct StratzHeroPoolEntry {
    int heroId = 0;
    int matchCount = 0;
    int winCount = 0;
};

struct StratzPlayerIntel {
    qint64 steamId = 0;
    QString displayName;
    QString avatarUrl;
    double winRate = -1.0;
    QVector<StratzHeroPoolEntry> heroes;
    bool ok = false;
    QString rawJson;
};

} // namespace gemsight::core

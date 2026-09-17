#pragma once

#include <QVector>

namespace gemsight::core {

struct DraftSnapshot {
    QVector<int> enemyHeroIds;
    QVector<qint64> enemySteamIds;
    QVector<int> allyHeroIds;
};

} // namespace gemsight::core

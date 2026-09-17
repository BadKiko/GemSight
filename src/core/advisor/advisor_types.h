#pragma once

#include <QObject>

namespace gemsight {
namespace core {

Q_NAMESPACE

enum class PickTier {
    Recommended = 0,
    Viable = 1,
    Neutral = 2,
    WarningHighRisk = 3,
    WarningMetaOnly = 4,
};
Q_ENUM_NS(PickTier)

} // namespace core
} // namespace gemsight

#pragma once

#include <QString>

namespace gemsight::core {

struct RoleGuess {
    QString label;
    double confidence = 0.0;
};

class RolePredictor {
public:
    static RoleGuess guessForPickOrder(int pickIndex);
};

} // namespace gemsight::core

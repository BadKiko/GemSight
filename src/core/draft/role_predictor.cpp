#include "core/draft/role_predictor.h"

namespace gemsight::core {

RoleGuess RolePredictor::guessForPickOrder(int pickIndex)
{
    static const struct {
        const char* label;
        double confidence;
    } kOrder[] = {
        {"Керри", 0.42},
        {"Мид", 0.38},
        {"Оффлейн", 0.35},
        {"Саппорт 4", 0.33},
        {"Саппорт 5", 0.36},
    };

    const int idx = qBound(0, pickIndex, 4);
    return {QString::fromUtf8(kOrder[idx].label), kOrder[idx].confidence};
}

} // namespace gemsight::core

#include "core/draft/role_predictor.h"

#include "core/heroes/hero_catalog.h"

#include <QHash>

namespace gemsight::core {

namespace {

QString roleFromHeroId(int heroId)
{
    static const QHash<int, QString> kRoles = {
        {1, QStringLiteral("Керри")},
        {2, QStringLiteral("Оффлейн")},
        {3, QStringLiteral("Саппорт 5")},
        {4, QStringLiteral("Керри")},
        {5, QStringLiteral("Саппорт 5")},
        {8, QStringLiteral("Керри")},
        {14, QStringLiteral("Саппорт 4")},
        {26, QStringLiteral("Саппорт 5")},
        {39, QStringLiteral("Мид")},
        {53, QStringLiteral("Оффлейн")},
        {76, QStringLiteral("Мид")},
        {86, QStringLiteral("Саппорт 4")},
        {110, QStringLiteral("Саппорт 5")},
    };
    return kRoles.value(heroId);
}

} // namespace

RoleGuess RolePredictor::guessForPickOrder(int pickIndex)
{
    static const struct {
        const char* label;
        double confidence;
    } kOrder[] = {
        {"Керри", 0.35},
        {"Мид", 0.33},
        {"Оффлейн", 0.32},
        {"Саппорт 4", 0.31},
        {"Саппорт 5", 0.34},
    };

    const int idx = qBound(0, pickIndex, 4);
    return {QString::fromUtf8(kOrder[idx].label), kOrder[idx].confidence};
}

RoleGuess RolePredictor::guessForHero(int heroId, int pickIndex)
{
    const QString heroRole = roleFromHeroId(heroId);
    if (!heroRole.isEmpty())
        return {heroRole, 0.72};

    return guessForPickOrder(pickIndex);
}

} // namespace gemsight::core

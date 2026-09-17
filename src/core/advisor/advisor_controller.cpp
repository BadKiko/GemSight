#include "core/advisor/advisor_controller.h"

#include "core/heroes/hero_catalog.h"

#include <algorithm>

namespace gemsight::core {

namespace {

int tierRank(PickTier tier)
{
    switch (tier) {
    case PickTier::Recommended:
        return 0;
    case PickTier::Viable:
        return 1;
    case PickTier::Neutral:
        return 2;
    case PickTier::WarningHighRisk:
        return 3;
    case PickTier::WarningMetaOnly:
        return 4;
    }
    return 2;
}

void sortRecommendations(QVector<PickRecommendationRow>& rows)
{
    std::stable_sort(rows.begin(), rows.end(), [](const PickRecommendationRow& a, const PickRecommendationRow& b) {
        const int ta = tierRank(a.tier);
        const int tb = tierRank(b.tier);
        if (ta != tb)
            return ta < tb;
        return a.score > b.score;
    });
}

} // namespace

AdvisorController::AdvisorController(QObject* parent)
    : QObject(parent)
{
}

void AdvisorController::setWeightMatchup(double v)
{
    if (qFuzzyCompare(m_weightMatchup, v))
        return;
    m_weightMatchup = v;
    emit weightsChanged();
}

void AdvisorController::setWeightMastery(double v)
{
    if (qFuzzyCompare(m_weightMastery, v))
        return;
    m_weightMastery = v;
    emit weightsChanged();
}

void AdvisorController::setAdvisorAggressiveMeta(bool v)
{
    if (m_aggressiveMeta == v)
        return;
    m_aggressiveMeta = v;
    emit weightsChanged();
}

void AdvisorController::loadDemoRecommendations()
{
    QVector<PickRecommendationRow> rows;

    auto rowFor = [](int heroId, double score, PickTier tier, const QString& msg, double matchup, double mastery,
                      int games, int games30d, bool signature) {
        PickRecommendationRow row;
        row.heroId = heroId;
        row.heroName = HeroCatalog::displayName(heroId);
        row.heroPortraitUrl = HeroCatalog::portraitUrl(heroId);
        row.score = score;
        row.tier = tier;
        row.message = msg;
        row.matchupAdvantage = matchup;
        row.personalMastery = mastery;
        row.gamesTotal = games;
        row.games30d = games30d;
        row.isSignature = signature;
        return row;
    };

    rows.push_back(rowFor(
        110,
        0.91,
        PickTier::Recommended,
        tr("Рекомендуется: комфортный сигнатурный пик с плюсом в драфте"),
        2.4,
        0.78,
        1240,
        42,
        true));
    rows.push_back(rowFor(
        76,
        0.84,
        PickTier::Viable,
        tr("Играбельно: лёгкий минус в матчапе, но сильный опыт на герое"),
        -1.1,
        0.71,
        890,
        28,
        true));
    rows.push_back(rowFor(
        16,
        0.62,
        PickTier::Neutral,
        tr("Нейтральный вариант по сумме матчапа и опыта"),
        0.3,
        0.41,
        210,
        9,
        false));
    rows.push_back(rowFor(
        53,
        0.58,
        PickTier::WarningHighRisk,
        tr("Высокий риск: сильные контрпики врага на сигнатуру"),
        -4.8,
        0.69,
        760,
        15,
        true));
    rows.push_back(rowFor(
        39,
        0.55,
        PickTier::WarningMetaOnly,
        tr("Мета-контрпик: осторожно — герой не отыгран"),
        3.2,
        0.12,
        18,
        2,
        false));

    sortRecommendations(rows);
    if (rows.size() > 8)
        rows.resize(8);

    m_model.resetRows(rows);
    emit recommendationsReady();
}

} // namespace gemsight::core

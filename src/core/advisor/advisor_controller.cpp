#include "core/advisor/advisor_controller.h"

#include "core/heroes/hero_catalog.h"
#include "core/meta/meta_matrix_store.h"

#include <algorithm>
#include <QHash>

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

void AdvisorController::evaluateFromDraft(const DraftSnapshot& snap, const MetaMatrixStore* matrices)
{
    if (!matrices || snap.enemyHeroIds.isEmpty())
        return;

    static const QHash<int, int> kLocalGames = {
        {110, 1240},
        {76, 890},
        {16, 210},
        {53, 760},
        {39, 18},
        {8, 420},
        {14, 980},
        {5, 310},
        {86, 150},
        {26, 540},
    };

    const QList<int> candidates = {110, 76, 16, 53, 39, 8, 14, 5, 86, 26};
    QVector<int> enemies;
    for (int id : snap.enemyHeroIds) {
        if (id > 0)
            enemies.push_back(id);
    }
    if (enemies.isEmpty())
        return;

    QVector<PickRecommendationRow> rows;
    const int minGames = 50;
    const double viableFloor = -1.5;
    const double highRiskCeiling = -4.0;
    const double metaOnlyFloor = 2.5;
    const double signatureMastery = 0.65;

    for (int heroId : candidates) {
        const double matchup = matrices->matchupAdvantage(heroId, enemies);
        const int games = kLocalGames.value(heroId, 0);
        const double mastery = games > 0 ? qMin(1.0, games / 1500.0) : 0.1;
        const bool signature = games >= 100 || mastery >= signatureMastery;
        const double score = m_weightMatchup * (matchup / 10.0) + m_weightMastery * mastery;

        PickTier tier = PickTier::Neutral;
        QString message = tr("Нейтральный вариант по сумме матчапа и опыта");

        if (signature && matchup >= 0.0) {
            tier = PickTier::Recommended;
            message = tr("Рекомендуется: комфортный сигнатурный пик с плюсом в драфте");
        } else if (signature && matchup >= viableFloor) {
            tier = PickTier::Viable;
            message = tr("Играбельно: лёгкий минус в матчапе, но сильный опыт на герое");
        } else if (games < minGames && matchup >= metaOnlyFloor && !m_aggressiveMeta) {
            tier = PickTier::WarningMetaOnly;
            message = tr("Мета-контрпик: осторожно — герой не отыгран");
        } else if (signature && matchup <= highRiskCeiling) {
            tier = PickTier::WarningHighRisk;
            message = tr("Высокий риск: сильные контрпики врага на сигнатуру");
        }

        if (games < minGames && !m_aggressiveMeta && (tier == PickTier::Recommended || tier == PickTier::Viable))
            tier = PickTier::WarningMetaOnly;

        PickRecommendationRow row;
        row.heroId = heroId;
        row.heroName = HeroCatalog::displayName(heroId);
        row.heroPortraitUrl = HeroCatalog::portraitUrl(heroId);
        row.score = score;
        row.tier = tier;
        row.message = message;
        row.matchupAdvantage = matchup;
        row.personalMastery = mastery;
        row.gamesTotal = games;
        row.games30d = 0;
        row.isSignature = signature;
        rows.push_back(row);
    }

    sortRecommendations(rows);
    if (rows.size() > 8)
        rows.resize(8);

    m_model.resetRows(rows);
    emit recommendationsReady();
}

} // namespace gemsight::core

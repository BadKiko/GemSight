#include "core/draft/draft_evaluator.h"

#include "core/heroes/hero_catalog.h"

namespace gemsight::core {

DraftEvaluator::DraftEvaluator(QObject* parent)
    : QObject(parent)
{
    reset();
}

void DraftEvaluator::reset()
{
    setWinProbability(0.5);
    m_threats.resetRows({});
    if (m_hint != QString()) {
        m_hint.clear();
        emit balanceHintChanged();
    }
}

void DraftEvaluator::loadDemoThreats()
{
    QVector<gemsight::LookaheadThreatRow> rows;
    const QList<int> ids = {39, 53, 76};
    const QList<QString> reasons = {
        tr("Сигнатура врага · сильный мид"),
        tr("Давление линий · глобальный герой"),
        tr("Контрпик по вашему пулу (демо)"),
    };
    const QList<double> scores = {0.82, 0.74, 0.68};

    for (int i = 0; i < ids.size(); ++i) {
        gemsight::LookaheadThreatRow row;
        row.heroId = ids.at(i);
        row.heroName = HeroCatalog::displayName(ids.at(i));
        row.heroPortraitUrl = HeroCatalog::portraitUrl(ids.at(i));
        row.reason = reasons.at(i);
        row.threatScore = scores.at(i);
        rows.push_back(row);
    }
    m_threats.resetRows(rows);
}

void DraftEvaluator::onEnemyPicksRevealed(int count)
{
    const int clamped = qBound(0, count, 5);
    const double delta = (clamped - 2.5) * 0.04;
    setWinProbability(0.5 - delta);

    if (clamped == 0)
        m_hint = tr("Драфт ещё не начался");
    else if (m_winProbability >= 0.52)
        m_hint = tr("Лёгкое преимущество по демо-оценке");
    else if (m_winProbability <= 0.48)
        m_hint = tr("Враги выглядят сильнее по демо-оценке");
    else
        m_hint = tr("Баланс близок к 50/50 (демо)");
    emit balanceHintChanged();
}

void DraftEvaluator::setWinProbability(double value)
{
    const double clamped = qBound(0.05, value, 0.95);
    if (qFuzzyCompare(m_winProbability, clamped))
        return;
    m_winProbability = clamped;
    emit liveWinProbabilityChanged();
}

} // namespace gemsight::core

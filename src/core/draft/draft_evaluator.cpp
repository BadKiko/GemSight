#include "core/draft/draft_evaluator.h"

namespace gemsight::core {

DraftEvaluator::DraftEvaluator(QObject* parent)
    : QObject(parent)
{
    reset();
}

void DraftEvaluator::reset()
{
    setWinProbability(0.5);
    if (m_hint != QString()) {
        m_hint.clear();
        emit balanceHintChanged();
    }
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

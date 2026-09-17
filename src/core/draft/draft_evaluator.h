#pragma once

#include "models/lookahead_threat_model.h"

#include <QObject>

namespace gemsight::core {

class DraftEvaluator : public QObject {
    Q_OBJECT
    Q_PROPERTY(double liveWinProbability READ liveWinProbability NOTIFY liveWinProbabilityChanged)
    Q_PROPERTY(QString balanceHint READ balanceHint NOTIFY balanceHintChanged)
    Q_PROPERTY(gemsight::LookaheadThreatModel* lookaheadThreats READ lookaheadThreats CONSTANT)

public:
    explicit DraftEvaluator(QObject* parent = nullptr);

    double liveWinProbability() const { return m_winProbability; }
    QString balanceHint() const { return m_hint; }
    gemsight::LookaheadThreatModel* lookaheadThreats() { return &m_threats; }

    void reset();
    void onEnemyPicksRevealed(int count);
    void loadDemoThreats();

signals:
    void liveWinProbabilityChanged();
    void balanceHintChanged();

private:
    void setWinProbability(double value);

    double m_winProbability = 0.5;
    QString m_hint;
    gemsight::LookaheadThreatModel m_threats;
};

} // namespace gemsight::core

#pragma once

#include <QObject>

namespace gemsight::core {

class DraftEvaluator : public QObject {
    Q_OBJECT
    Q_PROPERTY(double liveWinProbability READ liveWinProbability NOTIFY liveWinProbabilityChanged)
    Q_PROPERTY(QString balanceHint READ balanceHint NOTIFY balanceHintChanged)

public:
    explicit DraftEvaluator(QObject* parent = nullptr);

    double liveWinProbability() const { return m_winProbability; }
    QString balanceHint() const { return m_hint; }

    void reset();
    void onEnemyPicksRevealed(int count);

signals:
    void liveWinProbabilityChanged();
    void balanceHintChanged();

private:
    void setWinProbability(double value);

    double m_winProbability = 0.5;
    QString m_hint;
};

} // namespace gemsight::core

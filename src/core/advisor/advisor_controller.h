#pragma once

#include "core/advisor/pick_recommendation_model.h"
#include "core/draft/draft_snapshot.h"

#include <QObject>

namespace gemsight::core {

class MetaMatrixStore;

class AdvisorController : public QObject {
    Q_OBJECT
    Q_PROPERTY(PickRecommendationModel* pickRecommendations READ pickRecommendations CONSTANT)
    Q_PROPERTY(double weightMatchup READ weightMatchup WRITE setWeightMatchup NOTIFY weightsChanged)
    Q_PROPERTY(double weightMastery READ weightMastery WRITE setWeightMastery NOTIFY weightsChanged)
    Q_PROPERTY(bool advisorAggressiveMeta READ advisorAggressiveMeta WRITE setAdvisorAggressiveMeta NOTIFY weightsChanged)

public:
    explicit AdvisorController(QObject* parent = nullptr);

    PickRecommendationModel* pickRecommendations() { return &m_model; }
    double weightMatchup() const { return m_weightMatchup; }
    double weightMastery() const { return m_weightMastery; }
    bool advisorAggressiveMeta() const { return m_aggressiveMeta; }

    void setWeightMatchup(double v);
    void setWeightMastery(double v);
    void setAdvisorAggressiveMeta(bool v);

    Q_INVOKABLE void loadDemoRecommendations();
    void evaluateFromDraft(const DraftSnapshot& snap, const MetaMatrixStore* matrices);

signals:
    void recommendationsReady();
    void weightsChanged();

private:
    PickRecommendationModel m_model;
    double m_weightMatchup = 0.55;
    double m_weightMastery = 0.45;
    bool m_aggressiveMeta = false;
};

} // namespace gemsight::core

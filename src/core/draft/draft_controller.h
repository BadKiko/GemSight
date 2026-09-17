#pragma once

#include "core/cache/intel_cache.h"
#include "core/draft/draft_evaluator.h"
#include "core/draft/intel_gate.h"
#include "core/meta/meta_matrix_store.h"
#include "core/intel/player_intel_fetcher.h"
#include "core/stratz/stratz_player_intel_service.h"
#include "core/stratz/stratz_types.h"
#include "core/draft/draft_snapshot.h"
#include "models/enemy_team_model.h"
#include "models/ally_team_model.h"

#include <QObject>
#include <QTimer>

namespace gemsight::core {

class MatchSessionController;
class GsiServer;
class SettingsStore;

class DraftController : public QObject {
    Q_OBJECT
    Q_PROPERTY(gemsight::EnemyTeamModel* enemies READ enemies CONSTANT)
    Q_PROPERTY(gemsight::AllyTeamModel* allies READ allies CONSTANT)
    Q_PROPERTY(QString policyLabel READ policyLabel NOTIFY policyLabelChanged)
    Q_PROPERTY(QString statusLine READ statusLine NOTIFY statusLineChanged)
    Q_PROPERTY(bool gsiListening READ gsiListening NOTIFY gsiListeningChanged)
    Q_PROPERTY(gemsight::core::DraftEvaluator* evaluator READ evaluator CONSTANT)

public:
    explicit DraftController(QObject* parent = nullptr);

    gemsight::EnemyTeamModel* enemies() { return &m_enemies; }
    gemsight::AllyTeamModel* allies() { return &m_allies; }
    QString policyLabel() const { return m_policyLabel; }
    QString statusLine() const { return m_statusLine; }
    bool gsiListening() const;
    DraftEvaluator* evaluator() { return &m_evaluator; }

    void bindSession(MatchSessionController* session);
    void bindAdvisor(class AdvisorController* advisor);
    void bindSettings(SettingsStore* settings);
    void bindGsi(GsiServer* gsi);

    Q_INVOKABLE void startGsi();
    Q_INVOKABLE void simulateTurboDraft();

signals:
    void policyLabelChanged();
    void statusLineChanged();
    void gsiListeningChanged();

private:
    void refreshPolicyLabel();
    void setStatus(const QString& line);
    void onHeroPicked(int teamSlot, int heroId, const QString& heroName);
    void onEnemySteam(int teamSlot, qint64 steamId, const QString& name);
    void onStrategyTime();
    void schedulePlayerIntel(int teamSlot, qint64 steamId);
    void refreshAdvisor();
    DraftSnapshot buildSnapshot() const;
    void applyIntelToSlot(int teamSlot, qint64 steamId, const StratzPlayerIntel& stratz, const PlayerIntelResult& openDota);
    void applyRoleGuess(gemsight::EnemySlot& slot);
    void decorateHeroVisuals(gemsight::EnemySlot& slot, int heroId);
    void refreshEvaluatorFromEnemies();
    void populateDemoAllies();

    gemsight::EnemyTeamModel m_enemies;
    gemsight::AllyTeamModel m_allies;
    IntelCache m_intelCache;
    MetaMatrixStore m_metaMatrices;
    PlayerIntelFetcher m_intelFetcher;
    StratzPlayerIntelService m_stratzService;
    SettingsStore* m_settings = nullptr;
    bool m_demoAdvisor = false;
    IntelGate m_gate;
    DraftEvaluator m_evaluator;
    QString m_activePatch = QStringLiteral("current");
    class AdvisorController* m_advisor = nullptr;
    MatchSessionController* m_session = nullptr;
    GsiServer* m_gsi = nullptr;
    QString m_policyLabel;
    QString m_statusLine;
    QTimer m_debounce;
};

} // namespace gemsight::core

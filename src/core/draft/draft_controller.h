#pragma once

#include "core/draft/intel_gate.h"
#include "models/enemy_team_model.h"
#include "models/ally_team_model.h"

#include <QObject>
#include <QTimer>

namespace gemsight::core {

class MatchSessionController;
class GsiServer;

class DraftController : public QObject {
    Q_OBJECT
    Q_PROPERTY(gemsight::EnemyTeamModel* enemies READ enemies CONSTANT)
    Q_PROPERTY(gemsight::AllyTeamModel* allies READ allies CONSTANT)
    Q_PROPERTY(QString policyLabel READ policyLabel NOTIFY policyLabelChanged)
    Q_PROPERTY(QString statusLine READ statusLine NOTIFY statusLineChanged)
    Q_PROPERTY(bool gsiListening READ gsiListening NOTIFY gsiListeningChanged)

public:
    explicit DraftController(QObject* parent = nullptr);

    gemsight::EnemyTeamModel* enemies() { return &m_enemies; }
    gemsight::AllyTeamModel* allies() { return &m_allies; }
    QString policyLabel() const { return m_policyLabel; }
    QString statusLine() const { return m_statusLine; }
    bool gsiListening() const;

    void bindSession(MatchSessionController* session);
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
    void scheduleStratzStub(int teamSlot, qint64 steamId);

    gemsight::EnemyTeamModel m_enemies;
    gemsight::AllyTeamModel m_allies;
    IntelGate m_gate;
    MatchSessionController* m_session = nullptr;
    GsiServer* m_gsi = nullptr;
    QString m_policyLabel;
    QString m_statusLine;
    QTimer m_debounce;
};

} // namespace gemsight::core

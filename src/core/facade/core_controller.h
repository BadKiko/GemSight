#pragma once

#include "core/draft/draft_controller.h"
#include "core/gsi/gsi_server.h"
#include "core/session/match_session_controller.h"
#include "core/settings/settings_store.h"

#include <QObject>

namespace gemsight::core {

class CoreController : public QObject {
    Q_OBJECT
    Q_PROPERTY(SettingsStore* settings READ settings CONSTANT)
    Q_PROPERTY(DraftController* draft READ draft CONSTANT)
    Q_PROPERTY(MatchSessionController* session READ session CONSTANT)
    Q_PROPERTY(QString matchPhase READ matchPhase NOTIFY matchPhaseChanged)

public:
    static CoreController& instance();
    static void registerCoreTypes();

    SettingsStore* settings() { return &m_settings; }
    DraftController* draft() { return &m_draft; }
    MatchSessionController* session() { return &m_session; }
    QString matchPhase() const { return m_session.matchPhase(); }

signals:
    void matchPhaseChanged();

private:
    explicit CoreController(QObject* parent = nullptr);

    SettingsStore m_settings;
    MatchSessionController m_session;
    GsiServer m_gsi;
    DraftController m_draft;
};

} // namespace gemsight::core

#include "core/facade/core_controller.h"

#include "core/i18n/translation_service.h"
#include "models/enemy_team_model.h"

#include <QQmlEngine>

namespace gemsight::core {

CoreController& CoreController::instance()
{
    static CoreController self;
    return self;
}

CoreController::CoreController(QObject* parent)
    : QObject(parent)
{
    m_draft.bindSession(&m_session);
    m_draft.bindGsi(&m_gsi);
    m_draft.bindAdvisor(&m_advisor);
    connect(&m_session, &MatchSessionController::matchPhaseChanged, this, [this]() {
        emit matchPhaseChanged();
    });
}

void CoreController::registerCoreTypes()
{
    qmlRegisterSingletonInstance("GemSight.Core", 1, 0, "Core", &instance());
    qmlRegisterUncreatableType<gemsight::EnemyTeamModel>(
        "GemSight.Core",
        1,
        0,
        "EnemyTeamModel",
        QStringLiteral("Use Core.draft.enemies"));
}

} // namespace gemsight::core

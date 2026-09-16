#include "core/facade/core_controller.h"
#include "core/i18n/translation_service.h"
#include "core/settings/settings_store.h"

#include <QDir>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlError>
#include <QStyleHints>

#ifndef QT_QML_MATERIAL_IMPORT_PATH
#define QT_QML_MATERIAL_IMPORT_PATH ""
#endif

namespace {

void configureQmlEngine(QQmlApplicationEngine& engine)
{
    const QString appDir = QCoreApplication::applicationDirPath();
    engine.addImportPath(appDir + QStringLiteral("/qml"));
    engine.addImportPath(appDir + QStringLiteral("/qml_modules"));

    const QByteArray materialPathEnv = qgetenv("QT_QML_MATERIAL_IMPORT_PATH");
    const QString materialPath = materialPathEnv.isEmpty()
        ? QStringLiteral(QT_QML_MATERIAL_IMPORT_PATH)
        : QString::fromLocal8Bit(materialPathEnv);
    if (!materialPath.isEmpty()) {
        const QString resolved = QDir::isAbsolutePath(materialPath)
            ? materialPath
            : (appDir + QLatin1Char('/') + materialPath);
        engine.addImportPath(resolved);
    }
}

} // namespace

int main(int argc, char* argv[])
{
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(
        Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);

    QGuiApplication app(argc, argv);
    QCoreApplication::setApplicationName(QStringLiteral("GemSight"));
    QCoreApplication::setOrganizationName(QStringLiteral("GemSight"));

    gemsight::core::CoreController::registerCoreTypes();

    QQmlApplicationEngine engine;
    configureQmlEngine(engine);

    auto& translations = gemsight::core::TranslationService::instance();
    translations.setEngine(&engine);
    auto& core = gemsight::core::CoreController::instance();
    translations.applyLanguage(core.settings()->uiLanguage());

    QObject::connect(core.settings(), &gemsight::core::SettingsStore::uiLanguageChanged, &app,
                     [&translations, &core]() {
                         translations.applyLanguage(core.settings()->uiLanguage());
                     });

    QObject::connect(
        &engine,
        &QQmlEngine::warnings,
        &app,
        [](const QList<QQmlError>& errors) {
            for (const QQmlError& error : errors)
                qWarning().noquote() << error.toString();
        });

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(1); },
        Qt::QueuedConnection);

    engine.loadFromModule(QStringLiteral("gemsight"), QStringLiteral("Main"));

    core.draft()->startGsi();

    return app.exec();
}

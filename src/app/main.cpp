#include "core/facade/core_controller.h"
#include "core/i18n/translation_service.h"
#include "core/settings/settings_store.h"

#include <QDir>
#include <QFile>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlError>
#include <QStyleHints>

#ifdef QML_MATERIAL_STATIC_DEFINE
#include <QtPlugin>
Q_IMPORT_PLUGIN(Qcm_MaterialPlugin)
#endif

#ifndef QT_QML_MATERIAL_IMPORT_PATH
#define QT_QML_MATERIAL_IMPORT_PATH ""
#endif

namespace {

bool qmlMaterialPluginDllExists(const QString& importRoot)
{
    const QString dir = importRoot + QStringLiteral("/Qcm/Material/");
    const QStringList names = {
#ifdef Q_OS_WIN
        QStringLiteral("qml_materialplugin.dll"),
        QStringLiteral("libqml_materialplugin.dll"),
#else
        QStringLiteral("libqml_materialplugin.so"),
        QStringLiteral("qml_materialplugin.so"),
        QStringLiteral("libqml_materialplugin.dylib"),
#endif
    };
    for (const QString& name : names) {
        if (QFile::exists(dir + name))
            return true;
    }
    return false;
}

void addMaterialImportPath(QQmlApplicationEngine& engine, const QString& path)
{
    if (path.isEmpty())
        return;
    if (qmlMaterialPluginDllExists(path))
        engine.addImportPath(path);
}

void configureQmlEngine(QQmlApplicationEngine& engine)
{
    const QString appDir = QCoreApplication::applicationDirPath();
    engine.addImportPath(appDir + QStringLiteral("/qml"));
    addMaterialImportPath(engine, appDir + QStringLiteral("/qml_modules"));

    const QByteArray materialPathEnv = qgetenv("QT_QML_MATERIAL_IMPORT_PATH");
    const QString materialPath = materialPathEnv.isEmpty()
        ? QStringLiteral(QT_QML_MATERIAL_IMPORT_PATH)
        : QString::fromLocal8Bit(materialPathEnv);
    if (!materialPath.isEmpty()) {
        const QString resolved = QDir::isAbsolutePath(materialPath)
            ? materialPath
            : (appDir + QLatin1Char('/') + materialPath);
        addMaterialImportPath(engine, resolved);
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
    if (engine.rootObjects().isEmpty())
        return 1;

    core.draft()->startGsi();

    return app.exec();
}

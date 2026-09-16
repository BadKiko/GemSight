#include "core/i18n/translation_service.h"

#include <QCoreApplication>
#include <QLocale>
#include <QQmlEngine>
#include <QTranslator>

namespace gemsight::core {

TranslationService& TranslationService::instance()
{
    static TranslationService self;
    return self;
}

TranslationService::TranslationService(QObject* parent)
    : QObject(parent)
    , m_appTranslator(new QTranslator(this))
    , m_qtTranslator(new QTranslator(this))
{
}

void TranslationService::setEngine(QQmlEngine* engine)
{
    m_engine = engine;
}

void TranslationService::applyLanguage(const QString& languageCode)
{
    QCoreApplication::removeTranslator(m_appTranslator);
    QCoreApplication::removeTranslator(m_qtTranslator);

    const QString code = languageCode.isEmpty() ? QStringLiteral("ru") : languageCode;

    if (code != QStringLiteral("en")) {
        const QString resource = QStringLiteral(":/i18n/gemsight_%1.qm").arg(code);
        if (m_appTranslator->load(resource))
            QCoreApplication::installTranslator(m_appTranslator);
    }

    if (m_qtTranslator->load(QStringLiteral("qt_%1").arg(code), QStringLiteral(":/qttranslations")))
        QCoreApplication::installTranslator(m_qtTranslator);

    if (m_engine)
        m_engine->retranslate();
}

} // namespace gemsight::core

#pragma once

#include <QObject>

class QQmlEngine;
class QTranslator;

namespace gemsight::core {

class TranslationService : public QObject {
    Q_OBJECT
public:
    static TranslationService& instance();

    void setEngine(QQmlEngine* engine);
    void applyLanguage(const QString& languageCode);

private:
    explicit TranslationService(QObject* parent = nullptr);

    QQmlEngine* m_engine = nullptr;
    QTranslator* m_appTranslator = nullptr;
    QTranslator* m_qtTranslator = nullptr;
};

} // namespace gemsight::core

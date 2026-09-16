#pragma once

#include <QObject>
#include <QString>

namespace gemsight::core {

class SettingsStore : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString uiLanguage READ uiLanguage WRITE setUiLanguage NOTIFY uiLanguageChanged)

public:
    explicit SettingsStore(QObject* parent = nullptr);

    QString uiLanguage() const { return m_uiLanguage; }
    void setUiLanguage(const QString& lang);

signals:
    void uiLanguageChanged();

private:
    QString m_uiLanguage = QStringLiteral("ru");
};

} // namespace gemsight::core

#pragma once

#include <QObject>
#include <QString>

namespace gemsight::core {

class SettingsStore : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString uiLanguage READ uiLanguage WRITE setUiLanguage NOTIFY uiLanguageChanged)
    Q_PROPERTY(QString stratzApiToken READ stratzApiToken WRITE setStratzApiToken NOTIFY stratzApiTokenChanged)

public:
    explicit SettingsStore(QObject* parent = nullptr);

    QString uiLanguage() const { return m_uiLanguage; }
    void setUiLanguage(const QString& lang);

    QString stratzApiToken() const { return m_stratzToken; }
    void setStratzApiToken(const QString& token);

signals:
    void uiLanguageChanged();
    void stratzApiTokenChanged();

private:
    QString m_uiLanguage = QStringLiteral("ru");
    QString m_stratzToken;
};

} // namespace gemsight::core

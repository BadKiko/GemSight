#include "core/settings/settings_store.h"

#include <QSettings>

namespace gemsight::core {

SettingsStore::SettingsStore(QObject* parent)
    : QObject(parent)
{
    QSettings settings;
    m_uiLanguage = settings.value(QStringLiteral("ui/language"), QStringLiteral("ru")).toString();
    m_stratzToken = settings.value(QStringLiteral("stratz/apiToken")).toString();
}

void SettingsStore::setUiLanguage(const QString& lang)
{
    if (m_uiLanguage == lang)
        return;
    m_uiLanguage = lang;
    QSettings settings;
    settings.setValue(QStringLiteral("ui/language"), m_uiLanguage);
    emit uiLanguageChanged();
}

void SettingsStore::setStratzApiToken(const QString& token)
{
    if (m_stratzToken == token)
        return;
    m_stratzToken = token;
    QSettings settings;
    settings.setValue(QStringLiteral("stratz/apiToken"), m_stratzToken);
    emit stratzApiTokenChanged();
}

} // namespace gemsight::core

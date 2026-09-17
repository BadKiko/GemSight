#pragma once

#include <QObject>
#include <QString>
#include <QSqlDatabase>

namespace gemsight::core {

class IntelCache : public QObject {
    Q_OBJECT
public:
    explicit IntelCache(QObject* parent = nullptr);

    bool open(const QString& filePath = QString());
    QString avatarUrl(qint64 steamId, const QString& patch) const;
    void putPlayerProfile(qint64 steamId, const QString& patch, const QString& avatarUrl, const QString& payloadJson);

private:
    bool ensureSchema();

    QString m_path;
    QSqlDatabase m_db;
};

} // namespace gemsight::core

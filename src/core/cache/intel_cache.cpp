#include "core/cache/intel_cache.h"

#include <QDir>
#include <QFileInfo>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDateTime>
#include <QStandardPaths>

namespace gemsight::core {

IntelCache::IntelCache(QObject* parent)
    : QObject(parent)
{
}

bool IntelCache::open(const QString& filePath)
{
    QString path = filePath;
    if (path.isEmpty()) {
        const QString dir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        QDir().mkpath(dir);
        path = QDir(dir).filePath(QStringLiteral("intel_cache.sqlite"));
    }

    if (m_db.isOpen())
        m_db.close();

    m_path = path;
    m_db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), QStringLiteral("gemsight_intel"));
    m_db.setDatabaseName(m_path);
    if (!m_db.open())
        return false;

    return ensureSchema();
}

bool IntelCache::ensureSchema()
{
    QSqlQuery q(m_db);
    const char* sql =
        "CREATE TABLE IF NOT EXISTS player_profile ("
        "steam_id INTEGER NOT NULL,"
        "patch TEXT NOT NULL,"
        "avatar_url TEXT,"
        "payload_json TEXT,"
        "updated_at INTEGER NOT NULL,"
        "PRIMARY KEY (steam_id, patch)"
        ");";
    if (!q.exec(QString::fromUtf8(sql)))
        return false;

    q.exec(QStringLiteral(
        "CREATE TABLE IF NOT EXISTS hero_role_hist ("
        "hero_id INTEGER NOT NULL,"
        "bracket TEXT NOT NULL,"
        "role TEXT NOT NULL,"
        "weight REAL NOT NULL,"
        "PRIMARY KEY (hero_id, bracket, role)"
        ");"));
    return true;
}

QString IntelCache::avatarUrl(qint64 steamId, const QString& patch) const
{
    if (!m_db.isOpen() || steamId <= 0)
        return {};

    QSqlQuery q(m_db);
    q.prepare(QStringLiteral("SELECT avatar_url FROM player_profile WHERE steam_id = ? AND patch = ?"));
    q.addBindValue(steamId);
    q.addBindValue(patch);
    if (!q.exec() || !q.next())
        return {};
    return q.value(0).toString();
}

void IntelCache::putPlayerProfile(
    qint64 steamId,
    const QString& patch,
    const QString& avatarUrl,
    const QString& payloadJson)
{
    if (!m_db.isOpen() || steamId <= 0)
        return;

    QSqlQuery q(m_db);
    q.prepare(QStringLiteral(
        "INSERT INTO player_profile (steam_id, patch, avatar_url, payload_json, updated_at) "
        "VALUES (?, ?, ?, ?, ?) "
        "ON CONFLICT(steam_id, patch) DO UPDATE SET "
        "avatar_url = excluded.avatar_url, "
        "payload_json = excluded.payload_json, "
        "updated_at = excluded.updated_at"));
    q.addBindValue(steamId);
    q.addBindValue(patch);
    q.addBindValue(avatarUrl);
    q.addBindValue(payloadJson);
    q.addBindValue(QDateTime::currentSecsSinceEpoch());
    q.exec();
}

} // namespace gemsight::core

#include "core/intel/player_intel_fetcher.h"

#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>

namespace gemsight::core {

PlayerIntelFetcher::PlayerIntelFetcher(QObject* parent)
    : QObject(parent)
{
}

PlayerIntelResult PlayerIntelFetcher::fetchAvatarOpenDota(qint64 steamId) const
{
    PlayerIntelResult out;
    out.steamId = steamId;
    if (steamId <= 0)
        return out;

    QNetworkAccessManager nam;
    const QUrl url(QStringLiteral("https://api.opendota.com/api/players/%1").arg(steamId));
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::UserAgentHeader, QStringLiteral("GemSight/0.1"));

    QEventLoop loop;
    QNetworkReply* reply = nam.get(req);
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() != QNetworkReply::NoError) {
        reply->deleteLater();
        return out;
    }

    const QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    reply->deleteLater();
    if (!doc.isObject())
        return out;

    const QJsonObject obj = doc.object();
    const QString avatar = obj.value(QStringLiteral("avatarfull")).toString();
    const QString name = obj.value(QStringLiteral("personaname")).toString();
    if (avatar.isEmpty())
        return out;

    out.avatarUrl = avatar;
    out.displayName = name;
    out.ok = true;
    return out;
}

} // namespace gemsight::core

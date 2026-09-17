#include "core/stratz/stratz_graphql_client.h"

#include <QEventLoop>
#include <QJsonArray>
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>

namespace gemsight::core {

StratzGraphqlClient::Response StratzGraphqlClient::post(
    const QString& apiToken,
    const QString& query,
    const QJsonObject& variables) const
{
    Response out;
    if (apiToken.trimmed().isEmpty()) {
        out.errorMessage = QStringLiteral("STRATZ token missing");
        return out;
    }

    QNetworkAccessManager nam;
    QNetworkRequest req(QUrl(QStringLiteral("https://api.stratz.com/graphql")));
    req.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    req.setRawHeader("Authorization", QByteArray("Bearer ") + apiToken.trimmed().toUtf8());
    req.setRawHeader("User-Agent", "STRATZ_API");

    QJsonObject body;
    body.insert(QStringLiteral("query"), query);
    if (!variables.isEmpty())
        body.insert(QStringLiteral("variables"), variables);

    QEventLoop loop;
    QNetworkReply* reply = nam.post(req, QJsonDocument(body).toJson(QJsonDocument::Compact));
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() != QNetworkReply::NoError) {
        out.errorMessage = reply->errorString();
        reply->deleteLater();
        return out;
    }

    const QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    reply->deleteLater();
    if (!doc.isObject()) {
        out.errorMessage = QStringLiteral("Invalid JSON");
        return out;
    }

    const QJsonObject root = doc.object();
    if (root.contains(QStringLiteral("errors"))) {
        const QJsonArray errors = root.value(QStringLiteral("errors")).toArray();
        if (!errors.isEmpty())
            out.errorMessage = errors.first().toObject().value(QStringLiteral("message")).toString();
        else
            out.errorMessage = QStringLiteral("GraphQL error");
        return out;
    }

    out.data = root.value(QStringLiteral("data")).toObject();
    out.ok = true;
    return out;
}

} // namespace gemsight::core

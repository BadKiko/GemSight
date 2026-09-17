#pragma once

#include <QJsonObject>
#include <QString>

namespace gemsight::core {

class StratzGraphqlClient {
public:
    struct Response {
        QJsonObject data;
        QString errorMessage;
        bool ok = false;
    };

    Response post(const QString& apiToken, const QString& query, const QJsonObject& variables) const;
};

} // namespace gemsight::core

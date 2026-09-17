#pragma once

#include "core/stratz/stratz_graphql_client.h"
#include "core/stratz/stratz_types.h"

#include <QObject>

namespace gemsight::core {

class StratzPlayerIntelService : public QObject {
    Q_OBJECT
public:
    explicit StratzPlayerIntelService(QObject* parent = nullptr);

    StratzPlayerIntel fetchPlayer(const QString& apiToken, qint64 steamId) const;
    StratzPlayerIntel parsePlayerData(const QJsonObject& data, qint64 steamId) const;

private:

    StratzGraphqlClient m_client;
};

} // namespace gemsight::core

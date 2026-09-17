#include "core/stratz/stratz_player_intel_service.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QThread>

namespace gemsight::core {

namespace {

const char* kPlayerIntelQuery = R"(
query PlayerDraftIntel($id: Long!, $heroTake: Int!) {
  player(steamAccountId: $id) {
    steamAccountId
    name
    avatar
    winRate
    heroes(request: { take: $heroTake }) {
      heroId
      matchCount
      winCount
    }
  }
}
)";

} // namespace

StratzPlayerIntelService::StratzPlayerIntelService(QObject* parent)
    : QObject(parent)
{
}

StratzPlayerIntel StratzPlayerIntelService::fetchPlayer(const QString& apiToken, qint64 steamId) const
{
    QThread::msleep(150);

    StratzPlayerIntel out;
    out.steamId = steamId;

    QJsonObject vars;
    vars.insert(QStringLiteral("id"), steamId);
    vars.insert(QStringLiteral("heroTake"), 8);

    const auto response = m_client.post(apiToken, QString::fromUtf8(kPlayerIntelQuery), vars);
    if (!response.ok)
        return out;

    out.rawJson = QString::fromUtf8(QJsonDocument(response.data).toJson(QJsonDocument::Compact));
    return parsePlayerData(response.data, steamId);
}

StratzPlayerIntel StratzPlayerIntelService::parsePlayerData(const QJsonObject& data, qint64 steamId) const
{
    StratzPlayerIntel out;
    out.steamId = steamId;

    const QJsonObject player = data.value(QStringLiteral("player")).toObject();
    if (player.isEmpty())
        return out;

    out.displayName = player.value(QStringLiteral("name")).toString();
    out.avatarUrl = player.value(QStringLiteral("avatar")).toString();
    if (player.contains(QStringLiteral("winRate")))
        out.winRate = player.value(QStringLiteral("winRate")).toDouble();

    const QJsonArray heroes = player.value(QStringLiteral("heroes")).toArray();
    for (const QJsonValue& v : heroes) {
        if (!v.isObject())
            continue;
        const QJsonObject h = v.toObject();
        StratzHeroPoolEntry entry;
        entry.heroId = h.value(QStringLiteral("heroId")).toInt();
        entry.matchCount = h.value(QStringLiteral("matchCount")).toInt();
        entry.winCount = h.value(QStringLiteral("winCount")).toInt();
        if (entry.heroId > 0)
            out.heroes.push_back(entry);
    }

    out.ok = !out.displayName.isEmpty() || !out.avatarUrl.isEmpty() || !out.heroes.isEmpty();
    return out;
}

} // namespace gemsight::core

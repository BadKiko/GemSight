#pragma once

#include "core/draft/game_mode_policy.h"

#include <QJsonObject>
#include <QObject>
#include <QString>

namespace gemsight::core {

class MatchSessionController : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString matchPhase READ matchPhase NOTIFY matchPhaseChanged)
    Q_PROPERTY(QString gameModeLabel READ gameModeLabel NOTIFY gameModeLabelChanged)
    Q_PROPERTY(QString scoutPolicy READ scoutPolicy NOTIFY gameModePolicyChanged)

public:
    explicit MatchSessionController(QObject* parent = nullptr);

    QString matchPhase() const { return m_matchPhase; }
    QString gameModeLabel() const { return m_gameModeLabel; }
    GameModePolicy gameModePolicy() const { return m_policy; }
    QString scoutPolicy() const;

    Q_INVOKABLE void ingestPayload(const QJsonObject& payload);
    Q_INVOKABLE void simulateTurboDraft();

signals:
    void matchPhaseChanged();
    void gameModeLabelChanged();
    void gameModePolicyChanged();
    void draftHeroPicked(int teamSlot, int heroId, const QString& heroName);
    void enemySteamIdResolved(int teamSlot, qint64 steamId, const QString& name);
    void strategyTimeReached();

private:
    void setPhase(const QString& phase);
    void parseMap(const QJsonObject& map);
    void parseDraft(const QJsonObject& draft);
    void parseRosterPlayer(const QJsonObject& player, bool isEnemy);

    QString m_matchPhase = QStringLiteral("Idle");
    QString m_gameModeLabel;
    GameModePolicy m_policy = GameModePolicy::Unknown;
    bool m_strategyEmitted = false;
};

} // namespace gemsight::core

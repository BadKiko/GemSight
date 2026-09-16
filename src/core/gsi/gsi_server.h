#pragma once

#include <QObject>
#include <QJsonObject>

class QThread;

namespace gemsight::core {

class GsiServer : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool listening READ listening NOTIFY listeningChanged)
    Q_PROPERTY(quint16 port READ port CONSTANT)

public:
    explicit GsiServer(QObject* parent = nullptr);
    ~GsiServer() override;

    bool listening() const { return m_listening; }
    quint16 port() const { return m_port; }

    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();

signals:
    void listeningChanged();
    void payloadReceived(const QJsonObject& payload);
    void errorOccurred(const QString& message);

private:
    class Worker;
    Worker* m_worker = nullptr;
    QThread* m_thread = nullptr;
    bool m_listening = false;
    const quint16 m_port = 27042;
};

} // namespace gemsight::core

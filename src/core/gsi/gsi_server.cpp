#include "core/gsi/gsi_server.h"

#include <QHttpServer>
#include <QHttpServerRequest>
#include <QHttpServerResponse>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTcpServer>
#include <QThread>
#include <QtConcurrent>

namespace gemsight::core {

class GsiServer::Worker : public QObject {
    Q_OBJECT
public:
    explicit Worker(quint16 port, QObject* parent = nullptr)
        : QObject(parent)
        , m_port(port)
    {
    }

    void start()
    {
        if (m_http)
            return;

        m_tcp = new QTcpServer(this);
        if (!m_tcp->listen(QHostAddress::LocalHost, m_port)) {
            emit failed(QStringLiteral("GSI listen failed on port %1").arg(m_port));
            return;
        }

        m_http = new QHttpServer(this);
        m_http->route(QStringLiteral("/"), [this](const QHttpServerRequest& request) {
            const QByteArray body = request.body();
            (void)QtConcurrent::run([this, body]() {
                QJsonParseError err;
                const QJsonDocument doc = QJsonDocument::fromJson(body, &err);
                if (err.error != QJsonParseError::NoError || !doc.isObject())
                    return;
                const QJsonObject obj = doc.object();
                QMetaObject::invokeMethod(
                    this, [this, obj]() { emit payloadReady(obj); }, Qt::QueuedConnection);
            });
            return QHttpServerResponse(QHttpServerResponse::StatusCode::Ok);
        });

        if (!m_http->bind(m_tcp))
            emit failed(QStringLiteral("GSI HTTP bind failed"));
        else
            emit started();
    }

    void stop()
    {
        if (m_tcp)
            m_tcp->close();
        if (m_http) {
            m_http->deleteLater();
            m_http = nullptr;
        }
        m_tcp = nullptr;
        emit stopped();
    }

signals:
    void started();
    void stopped();
    void failed(const QString& message);
    void payloadReady(const QJsonObject& payload);

private:
    quint16 m_port;
    QTcpServer* m_tcp = nullptr;
    QHttpServer* m_http = nullptr;
};

GsiServer::GsiServer(QObject* parent)
    : QObject(parent)
{
}

GsiServer::~GsiServer()
{
    stop();
}

void GsiServer::start()
{
    if (m_thread)
        return;

    m_thread = new QThread(this);
    m_worker = new Worker(m_port);
    m_worker->moveToThread(m_thread);

    connect(m_thread, &QThread::finished, m_worker, &QObject::deleteLater);
    connect(m_worker, &Worker::started, this, [this]() {
        m_listening = true;
        emit listeningChanged();
    });
    connect(m_worker, &Worker::stopped, this, [this]() {
        m_listening = false;
        emit listeningChanged();
    });
    connect(m_worker, &Worker::failed, this, &GsiServer::errorOccurred);
    connect(m_worker, &Worker::payloadReady, this, &GsiServer::payloadReceived);

    connect(m_thread, &QThread::started, m_worker, &Worker::start);
    m_thread->start();
}

void GsiServer::stop()
{
    if (!m_thread)
        return;

    QMetaObject::invokeMethod(m_worker, &Worker::stop, Qt::QueuedConnection);
    m_thread->quit();
    m_thread->wait(3000);
    m_thread->deleteLater();
    m_thread = nullptr;
    m_worker = nullptr;
    m_listening = false;
    emit listeningChanged();
}

} // namespace gemsight::core

#include "gsi_server.moc"

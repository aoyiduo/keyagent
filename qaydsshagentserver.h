#ifndef QAYDSSHAGENTSERVER_H
#define QAYDSSHAGENTSERVER_H

#include <QObject>
#include <QPointer>
#include <QList>

class QTcpServer;
class QTcpSocket;
class QAydSshAgentClient;
class QAydSshAgentServer : public QObject
{
    Q_OBJECT
public:
    explicit QAydSshAgentServer(int port, QObject *parent = nullptr);
    bool listen(int port);
private slots:
    void onNewConnection();
    void onClientDestroyed();
private:
    QPointer<QTcpServer> m_server;
    QList<QPointer<QAydSshAgentClient>> m_clients;
};

#endif // QAYDSSHAGENTSERVER_H

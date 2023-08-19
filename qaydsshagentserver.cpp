#include "qaydsshagentserver.h"
#include "qaydsshagentclient.h"

#include <QTcpServer>
#include <QDebug>
#include <QTcpSocket>

QAydSshAgentServer::QAydSshAgentServer(int port, QObject *parent)
    : QObject(parent)
{

}

bool QAydSshAgentServer::listen(int port)
{
    if(m_server) {
        m_server->close();
        m_server->deleteLater();
    }
    m_server = new QTcpServer(this);
    QObject::connect(m_server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
    if(!m_server->listen(QHostAddress::Any, port)) {
        qInfo() << m_server->errorString();
        return false;
    }
    return true;
}

void QAydSshAgentServer::onNewConnection()
{
    while(m_server->hasPendingConnections()) {
        QTcpSocket *tcp = m_server->nextPendingConnection();
        QAydSshAgentClient *client = new QAydSshAgentClient(tcp, this);
        QObject::connect(client, SIGNAL(destroyed()), this, SLOT(onClientDestroyed()));
        m_clients.append(client);
    }
}

void QAydSshAgentServer::onClientDestroyed()
{
    QAydSshAgentClient *client = qobject_cast<QAydSshAgentClient*>(sender());
    m_clients.removeAll(client);
}

#include "qaydsshagentclient.h"

#include <QTcpSocket>
#include <QLocalSocket>
#include <QProcessEnvironment>

QAydSshAgentClient::QAydSshAgentClient(QTcpSocket *tcp, QObject *parent)
    : QObject(parent)
    , m_tcp(tcp)
{
    QObject::connect(tcp, SIGNAL(disconnected()), this, SLOT(deleteLater()));
    QObject::connect(tcp, SIGNAL(readyRead()), this, SLOT(onTcpReadyRead()));
    QProcessEnvironment envs = QProcessEnvironment::systemEnvironment();
    QString v = envs.value("SSH_AUTH_SOCK");
    if(v.isEmpty()) {
        deleteLater();
    }else{
        m_local = new QLocalSocket(this);
        QObject::connect(m_local, SIGNAL(readyRead()), this, SLOT(onLocalReadyRead()));
        QObject::connect(m_local, SIGNAL(disconnected()), this, SLOT(deleteLater()));
        QObject::connect(m_local, SIGNAL(error(QLocalSocket::LocalSocketError)), this, SLOT(deleteLater()));
        m_local->connectToServer(v);
    }
}

QAydSshAgentClient::~QAydSshAgentClient()
{
    if(m_tcp) {
        m_tcp->deleteLater();
    }
    if(m_local) {
        m_local->deleteLater();
    }
}

void QAydSshAgentClient::onTcpReadyRead()
{
    QByteArray all = m_tcp->readAll();
    m_local->write(all);
}

void QAydSshAgentClient::onLocalReadyRead()
{
    QByteArray all = m_local->readAll();
    m_tcp->write(all);
}

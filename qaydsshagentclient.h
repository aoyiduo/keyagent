#ifndef QAYDSSHAGENTCLIENT_H
#define QAYDSSHAGENTCLIENT_H

#include <QObject>
#include <QPointer>

class QTcpSocket;
class QLocalSocket;
class QAydSshAgentClient : public QObject
{
    Q_OBJECT
public:
    explicit QAydSshAgentClient(QTcpSocket *tcp, QObject *parent = nullptr);
    virtual ~QAydSshAgentClient();

private slots:
    void onTcpReadyRead();
    void onLocalReadyRead();

private:
    QPointer<QTcpSocket> m_tcp;
    QPointer<QLocalSocket> m_local;
};

#endif // QAYDSSHAGENTCLIENT_H

#include <QCoreApplication>
#include <QtDebug>
#include <QDir>
#include <QSettings>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QProcessEnvironment>
#include <QProcess>

#include "qaydsshagentclient.h"
#include "qaydsshagentserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("keyagent");
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("keyagent run as a proxy for ssh-agent.");
    parser.addHelpOption();
    QCommandLineOption optPort(QStringList() << "p" << "port", "server port", "", QString::number(1200));
    parser.addOption(optPort);
    parser.process(app);

    QProcessEnvironment envs = QProcessEnvironment::systemEnvironment();
    QString v = envs.value("SSH_AUTH_SOCK");
    if(v.isEmpty()) {
        qInfo() << "Unable to find a valid environment named SSH_AUTH_SOCK";
        return -1;
    }
    qInfo() << "SSH_AUTH_SOCK" << v;
    int iport = parser.value(optPort).toInt();
    QAydSshAgentServer server(iport);
    if(!server.listen(iport)) {
        qInfo() << "failed to start server.";
        return -2;
    }
    qInfo() << "server is running...";
    return app.exec();
}

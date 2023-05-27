#ifndef SERVERMAN_H
#define SERVERMAN_H

#include <QApplication>
#include <QThread>
#include <QString>
#include <QByteArray>
#include <QDataStream>
#include <QTcpSocket>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVector>
#include <QFile>

enum NetworkState
{
    Online,
    Offline
};

class ServerMan : public QObject
{
    Q_OBJECT

public:
    ServerMan();
    ~ServerMan();

    NetworkState getNetworkState();

signals:
    void userNameExistResult(bool result);
    void idExistResult(bool result);
    void command(QString cmd);
    void notConnected();
    void databaseUpdated();
    void dirUpdated();

private slots:
    void sendDataProc(QString filename);
    void commandProc(QString cmd);
    void newMessage();
    void messageAsDataProc();
    void messageAsCommandProc();
    void run();
    void notConnectedProc();

signals:
    void sendData(QString filename);
    void messageAsData();
    void messageAsCommand();

private:
    QTcpSocket *socket;
    QSqlDatabase *tempDB;
    QSqlQuery *tempDBQuery;
    QVector<QString> job;
    QThread *thread;
    NetworkState ns = NetworkState::Offline;
};

#endif // SERVERMAN_H

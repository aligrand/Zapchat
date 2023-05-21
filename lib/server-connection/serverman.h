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

class ServerMan
{
public:
    ServerMan();
    ~ServerMan();

signals:
    void isUserNameExist(QString un);
    void userNameExistResult(bool result);
    void command(QString cmd);
    void downloadData(QString address);
    void notConnected();
    void databaseUpdated();

private slots:
    void userNameExistCheck(QString un);
    void sendDataProc(QString address);
    void sendDataBaseProc(QString address);
    void commandProc(QString cmd);
    void downloadDataProc(QString address);
    void newMessage();
    void messageAsDataProc();
    void messageAsDataBaseProc();
    void messageAsCommandProc();
    void run();
    void notConnectedProc();

signals:
    void sendData(QString address);
    void sendDataBase(QString address);
    void messageAsData();
    void messageAsDataBase();
    void messageAsCommand();

private:
    QTcpSocket *socket;
    QSqlDatabase *db;
    QSqlQuery *dbQuery;
    QVector<QString> job;
    QThread *thread;
};

#endif // SERVERMAN_H

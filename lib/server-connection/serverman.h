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

class ServerMan : QObject
{
public:
    ServerMan();
    ~ServerMan();

signals:
    void isUserNameExist(QString un);
    void isIdExist(QString id);
    void userNameExistResult(bool result);
    void idExistResult(bool result);
    void command(QString cmd);
    void downloadData(QString fileName, QString address);
    void notConnected();
    void databaseUpdated();

private slots:
    void userNameExistCheck(QString un);
    void idExistCheck(QString id);
    void sendDataProc(QString address);
    void sendDataBaseProc(QString address);
    void commandProc(QString cmd);
    void downloadDataProc(QString fileName, QString address);
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
    QSqlDatabase *tempDB;
    QSqlQuery *tempDBQuery;
    QVector<QString> job;
    QThread *thread;
};

#endif // SERVERMAN_H

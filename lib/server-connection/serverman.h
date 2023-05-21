#ifndef SERVERMAN_H
#define SERVERMAN_H

#include <QApplication>
#include <QString>
#include <QByteArray>
#include <QDataStream>
#include <QTcpSocket>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QJsonDocument>
#include <QJsonObject>

class ServerMan
{
public:
    ServerMan(QString username = "", QString password = "");
    ~ServerMan();

signals:
    void isUserNameExist(QString un);
    void userNameExistResult(bool result);
    void sendData(QString address);
    void sendDataBase(QString address);
    void sendCommand(QString cmd);
    void downloadData(QString address);
    void notConnected();
    void databaseUpdated();

private slots:
    void userNameExistCheck(QString un);
    void sendDataProc(QString address);
    void sendDataBaseProc(QString address);
    void sendCommandProc(QString cmd);
    void downloadDataProc(QString address);
    void newMessage();
    void messageAsDataProc();
    void messageAsDataBaseProc();
    void messageAsCommandProc();

signals:
    void messageAsData();
    void messageAsDataBase();
    void messageAsCommand();

private:
    QTcpSocket *socket;
    QSqlDatabase *db;
    QSqlQuery *dbQuery;
    QJsonDocument *job;
};

#endif // SERVERMAN_H

#ifndef SERVERMAN_H
#define SERVERMAN_H

#include <QApplication>
#include <QString>
#include <QByteArray>
#include <QDataStream>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QSqlQuery>
#include <QVector>
#include <QFile>
#include <QTimer>
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QDebug>

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
    void userNameExistResult(bool result, QString un);
    void idExistResult(bool result, QString id);
    void loginResult(int result);
    void command(QString cmd);
    void notConnected();
    void connected();
    void databaseUpdated(QString additionalInfo);
    void dirUpdated();

private slots:
    void sendDataProc(QByteArray sData);
    void commandProc(QString cmd);
    void newMessage();
    void messageAsDataProc(QByteArray rData);
    void messageAsCommandProc(QByteArray rData);
    void sendRun();
    void notConnectedProc();
    void dataArrivedProc(int channel, qint64 bytes);
    void connectedProc();
    void reConnect();

signals:
    void sendData(QByteArray sData);
    void messageAsData(QByteArray rData);
    void messageAsCommand(QByteArray rData);
    void dataArrived();
    void start_sendRun();

private:
    QTcpSocket *socket;
    QVector<QString> job;
    QVector<QString> forceJob;
    QVector<QString> job_delPending;
    QVector<QString> forceJob_delPending;
    NetworkState ns = NetworkState::Offline;
    QTimer readyReadTimer;
    QTimer hostConn;
    bool is_login = false;
    int dataSize = INT_MAX;
    char job_order; // f->force , j->not force
};

#endif // SERVERMAN_H

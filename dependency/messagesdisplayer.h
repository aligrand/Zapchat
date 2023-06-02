#ifndef MESSAGESDISPLAYER_H
#define MESSAGESDISPLAYER_H

#include <QWidget>
#include <QVector>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSqlQuery>
#include <QScrollBar>

#include "../lib/server-connection/serverman.h"
#include "../src/messagewidget.h"

extern ServerMan *server;
extern QString myUsername;

namespace Ui {
class MessagesDisplayer;
}

class MessagesDisplayer : public QWidget
{
    Q_OBJECT

public:
    explicit MessagesDisplayer(QString queryCondition, QString room_id = "", QWidget *parent = nullptr);
    ~MessagesDisplayer();

signals:
    void recreateList();

private slots:
    void updateMessagesQuery(QString additionalInfo);
    void loadMessage(int value);

private:
    void delListItem(QHBoxLayout *layout);

private:
    Ui::MessagesDisplayer *ui;
    QVector<QHBoxLayout *> messagesList; // max size = 50 element
    QSqlQuery sqlQuery;
    QString rid, qc;
    QVBoxLayout *contentLayout = new QVBoxLayout;
    int lastVScrollBarValue;
};

#endif // MESSAGESDISPLAYER_H

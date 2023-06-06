#include "roomwidget.h"
#include "ui_roomwidget.h"

RoomWidget::RoomWidget(QString roomID, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RoomWidget)
{
    ui->setupUi(this);

    connect(server, &ServerMan::databaseUpdated, this, &RoomWidget::newMessagesCame);
    connect(this, &RoomWidget::clicked, this, &RoomWidget::clickedProc);

    rID = roomID;

    newMessagesCame("messages");

    QSqlQuery sqlQuery;

    sqlQuery.prepare("SELECT * FROM rooms WHERE roomID=?");
    sqlQuery.addBindValue(roomID);
    sqlQuery.exec();

    if (sqlQuery.value("type").toInt() == 0)
    {
        sqlQuery.prepare("SELECT * FROM participants WHERE roomID=? AND NOT userID=?");
        sqlQuery.addBindValue(roomID);
        sqlQuery.addBindValue(myUsername);
        sqlQuery.exec();

        ui->roomPic->setPixmap(QPixmap("Chache/" + sqlQuery.value("photoADDRESS").toString()));
        ui->roomName->setText(sqlQuery.value("name").toString());

        sqlQuery.prepare("SELECT text FROM messages WHERE roomID=?");
        sqlQuery.addBindValue(roomID);
        sqlQuery.exec();

        ui->lastMessage->setText(sqlQuery.value("text").toString());
    }
    else
    {
        ui->roomPic->setPixmap(QPixmap("Chache/" + sqlQuery.value("photoADDRESS").toString()));
        ui->roomName->setText(sqlQuery.value("name").toString());

        sqlQuery.prepare("SELECT text FROM messages WHERE roomID=?");
        sqlQuery.addBindValue(roomID);
        sqlQuery.exec();

        ui->lastMessage->setText(sqlQuery.value("text").toString());
    }
}

RoomWidget::~RoomWidget()
{
    delete ui;
}

void RoomWidget::newMessagesCame(QString additionalInfo)
{
    if (additionalInfo != "messages")
    {
        return;
    }

    QSqlQuery sqlQuery;

    sqlQuery.prepare("SELECT count FROM new_messages WHERE roomID=?");
    sqlQuery.addBindValue(rID);
    sqlQuery.exec();

    delete newMCount;

    newMCount = new QLabel(this);

    newMCount->setText(sqlQuery.value("count").toString());
    newMCount->setStyleSheet("background-color: rgb(0, 85, 255);");
    newMCount->setGeometry(0, 0, 20, 20);

    newMCount->show();
}

void RoomWidget::clickedProc(QString roomId)
{
    QSqlQuery sqlQuery;

    sqlQuery.prepare("UPDATE new_messages SET count=0 WHERE roomID=?");
    sqlQuery.addBindValue(rID);
    sqlQuery.exec();

    delete newMCount;
}

void RoomWidget::on_roomName_linkActivated(const QString &link)
{
    emit clicked(rID);
}

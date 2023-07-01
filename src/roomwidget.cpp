#include "roomwidget.h"
#include "ui_roomwidget.h"

RoomWidget::RoomWidget(QString roomID, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RoomWidget)
{
    ui->setupUi(this);

    connect(server, &ServerMan::databaseUpdated, this, &RoomWidget::newMessagesCame);
    connect(server, &ServerMan::databaseUpdated, this, &RoomWidget::roomUpdated);
    connect(this, &RoomWidget::clicked, this, &RoomWidget::clickedProc);

    rID = roomID;

    newMessagesCame("messages-");

    QSqlQuery sqlQuery;

    sqlQuery.prepare("SELECT * FROM rooms WHERE id=?");
    sqlQuery.addBindValue(roomID);
    sqlQuery.exec();
    sqlQuery.first();

    if (sqlQuery.value("type").toInt() == 0)
    {
        sqlQuery.prepare("SELECT * FROM participants INNER JOIN users "
                         "ON users.username=participants.userID "
                         "WHERE participants.roomID=? AND NOT participants.userID=?");
        sqlQuery.addBindValue(roomID);
        sqlQuery.addBindValue(myUsername);
        sqlQuery.exec();
        sqlQuery.first();

        if (!sqlQuery.value("photoADDRESS").toString().isEmpty())
        {
            ui->roomPic->setPixmap(QPixmap("Profiles/" + sqlQuery.value("photoADDRESS").toString()));
        }

        ui->roomName->setText("<a style=\"text-decoration:none;color:black;\" href=\"#\">" +
                              sqlQuery.value("name").toString() + "</a>");

        sqlQuery.prepare("SELECT text FROM messages WHERE roomID=?");
        sqlQuery.addBindValue(roomID);
        sqlQuery.exec();
        if (sqlQuery.last())
        {
            ui->lastMessage->setText(sqlQuery.value("text").toString());
        }
    }
    else
    {
        if (!sqlQuery.value("photoADDRESS").toString().isEmpty())
        {
            ui->roomPic->setPixmap(QPixmap("Profiles/" + sqlQuery.value("photoADDRESS").toString()));
        }

        ui->roomName->setText("<a style=\"text-decoration:none;color:black;\" href=\"#\">" +
                              sqlQuery.value("name").toString() + "</a>");

        sqlQuery.prepare("SELECT text FROM messages WHERE roomID=?");
        sqlQuery.addBindValue(roomID);
        sqlQuery.exec();
        if (sqlQuery.last())
        {
            ui->lastMessage->setText(sqlQuery.value("text").toString());
        }
    }
}

RoomWidget::~RoomWidget()
{
    delete ui;
    delete newMCount;
}

void RoomWidget::newMessagesCame(QString additionalInfo)
{
    QSqlQuery sqlQuery;

    if (additionalInfo.split("-").first() == "messages")
    {
        sqlQuery.prepare("SELECT count FROM new_messages WHERE roomID=?");
        sqlQuery.addBindValue(rID);
        sqlQuery.exec();
        sqlQuery.first();

        if (sqlQuery.value("count").toInt() == 0)
        {
            return;
        }

        delete newMCount;

        newMCount = new QLabel(this);

        newMCount->setText(sqlQuery.value("count").toString());
        newMCount->setStyleSheet("background-color: rgb(0, 85, 255);");
        newMCount->setGeometry(0, 0, 20, 20);

        newMCount->show();
    }
}

void RoomWidget::roomUpdated(QString additionalInfo)
{
    QSqlQuery sqlQuery;

    if (additionalInfo == "participants")
    {
        sqlQuery.prepare("SELECT * FROM rooms WHERE id=?");
        sqlQuery.addBindValue(rID);
        sqlQuery.exec();
        sqlQuery.first();

        if (sqlQuery.value("type").toInt() == 0)
        {
            sqlQuery.prepare("SELECT * FROM participants INNER JOIN users "
                             "ON users.username=participants.userID "
                             "WHERE participants.roomID=? AND NOT participants.userID=?");
            sqlQuery.addBindValue(rID);
            sqlQuery.addBindValue(myUsername);
            sqlQuery.exec();
            sqlQuery.first();

            if (!sqlQuery.value("photoADDRESS").toString().isEmpty())
            {
                ui->roomPic->setPixmap(QPixmap("Profiles/" + sqlQuery.value("photoADDRESS").toString()));
            }

            ui->roomName->setText("<a style=\"text-decoration:none;color:black;\" href=\"#\">" +
                                  sqlQuery.value("name").toString() + "</a>");
        }
    }
    else if (additionalInfo == "rooms-edit")
    {
        sqlQuery.prepare("SELECT * FROM rooms WHERE id=?");
        sqlQuery.addBindValue(rID);
        sqlQuery.exec();
        sqlQuery.first();

        if (!sqlQuery.value("photoADDRESS").toString().isEmpty())
        {
            ui->roomPic->setPixmap(QPixmap("Profiles/" + sqlQuery.value("photoADDRESS").toString()));
        }

        ui->roomName->setText("<a style=\"text-decoration:none;color:black;\" href=\"#\">" +
                              sqlQuery.value("name").toString() + "</a>");

        sqlQuery.prepare("SELECT text FROM messages WHERE roomID=?");
        sqlQuery.addBindValue(rID);
        sqlQuery.exec();
        if (sqlQuery.last())
        {
            ui->lastMessage->setText(sqlQuery.value("text").toString());
        }
    }
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

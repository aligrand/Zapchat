#include "roominfopanel.h"
#include "ui_roominfopanel.h"

RoomInfoPanel::RoomInfoPanel(int type, QString id, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RoomInfoPanel)
{
    ui->setupUi(this);

    QSqlQuery sqlQuery;

    if (type == 1)
    {
        sqlQuery.prepare("SELECT * FROM participants WHERE roomID=? AND NOT userID=?");
        sqlQuery.addBindValue(id);
        sqlQuery.addBindValue(myUsername);
        sqlQuery.exec();
        sqlQuery.first();

        QString uid = sqlQuery.value("userID").toString();

        sqlQuery.prepare("SELECT * FROM users WHERE username=?");
        sqlQuery.addBindValue(uid);
        sqlQuery.exec();
        sqlQuery.first();

        ui->nameField->setText(sqlQuery.value("name").toString());
        ui->IDField->setText(sqlQuery.value("username").toString());
        ui->infoField->setText(sqlQuery.value("info").toString());
        ui->pofile_pic->setPixmap(QPixmap("Profiles/" + sqlQuery.value("photoADDRESS").toString()));
        ui->emailField->setText(sqlQuery.value("emailADDRESS").toString());
        ui->pnField->setText(sqlQuery.value("phoneNumber").toString());
    }
    else if (type == 0)
    {
        QSqlQuery sqlQuery;
        sqlQuery.prepare("SELECT * FROM rooms WHERE id=?");
        sqlQuery.addBindValue(id);
        sqlQuery.exec();
        sqlQuery.first();

        ui->nameField->setText(sqlQuery.value("name").toString());
        ui->IDField->setText(sqlQuery.value("id").toString());
        ui->infoField->setText(sqlQuery.value("info").toString());
        ui->pofile_pic->setPixmap(QPixmap("Profiles/" + sqlQuery.value("photoADDRESS").toString()));
        delete ui->groupBox_3;
        delete ui->groupBox_4;
    }
    else
    {
        QSqlQuery sqlQuery;
        sqlQuery.prepare("SELECT * FROM users WHERE username=?");
        sqlQuery.addBindValue(id);
        sqlQuery.exec();
        sqlQuery.first();

        ui->nameField->setText(sqlQuery.value("name").toString());
        ui->IDField->setText(sqlQuery.value("username").toString());
        ui->infoField->setText(sqlQuery.value("info").toString());
        ui->pofile_pic->setPixmap(QPixmap("Profiles/" + sqlQuery.value("photoADDRESS").toString()));
        ui->emailField->setText(sqlQuery.value("emailADDRESS").toString());
        ui->pnField->setText(sqlQuery.value("phoneNumber").toString());
    }
}

RoomInfoPanel::~RoomInfoPanel()
{
    delete ui;
}

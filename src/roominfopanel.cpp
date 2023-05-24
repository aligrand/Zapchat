#include "roominfopanel.h"
#include "ui_roominfopanel.h"

RoomInfoPanel::RoomInfoPanel(bool isUser, QString id, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RoomInfoPanel)
{
    ui->setupUi(this);

    if (isUser)
    {
        QSqlQuery sqlQuery;
        sqlQuery.prepare("SELECT * FROM users WHERE id=?");
        sqlQuery.addBindValue(id);
        sqlQuery.exec();

        ui->nameField->setText(sqlQuery.value("name").toString());
        ui->IDField->setText(sqlQuery.value("username").toString());
        ui->infoField->setText(sqlQuery.value("info").toString());
        ui->pofile_pic->setPixmap(QPixmap("Cache/" + sqlQuery.value("photoADDRESS").toString()));
        ui->emailField->setText(sqlQuery.value("emailADDRESS").toString());
        ui->pnField->setText(sqlQuery.value("phoneNumber").toString());
    }
    else
    {
        QSqlQuery sqlQuery;
        sqlQuery.prepare("SELECT * FROM rooms WHERE id=?");
        sqlQuery.addBindValue(id);
        sqlQuery.exec();

        ui->nameField->setText(sqlQuery.value("name").toString());
        ui->IDField->setText(sqlQuery.value("id").toString());
        ui->infoField->setText(sqlQuery.value("info").toString());
        ui->pofile_pic->setPixmap(QPixmap("Cache/" + sqlQuery.value("photoADDRESS").toString()));
        delete ui->emailField;
        delete ui->pnField;
    }
}

RoomInfoPanel::~RoomInfoPanel()
{
    delete ui;
}

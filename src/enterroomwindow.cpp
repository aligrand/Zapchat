#include "enterroomwindow.h"
#include "ui_enterroomwindow.h"

EnterRoomWindow::EnterRoomWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EnterRoomWindow)
{
    ui->setupUi(this);

    connect(server, &ServerMan::userNameExistResult, this, &EnterRoomWindow::enterRoom);
    connect(server, &ServerMan::idExistResult, this, &EnterRoomWindow::enterRoom);
}

EnterRoomWindow::~EnterRoomWindow()
{
    delete ui;
}

void EnterRoomWindow::on_goto_chat_button_clicked()
{
    if (ui->id_le->text() == "")
    {
        QMessageBox::critical(this, "Error", "You must enter ID");
    }

    if (ui->comboBox->currentText() == "Group") {
        isGroup = true;

        emit server->isIdExist(ui->id_le->text());
    }
    else {
        isGroup = false;

        emit server->isUserNameExist(ui->id_le->text());
    }
}

void EnterRoomWindow::enterRoom(bool result)
{
    QSqlQuery sqlQuery;

    sqlQuery.prepare("SELECT ")

    if (result)
    {
        if (isGroup)
        {
            sqlQuery.prepare("INSERT TO participants (userID, roomID, role) VALUES (?, ?, ?)");
            sqlQuery.addBindValue(myUsername);
            sqlQuery.addBindValue(ui->id_le->text());
            sqlQuery.addBindValue("G");

            sqlQuery.exec();

            emit server->command("ENTER-ROOM " + ui->id_le->text());
        }
        else
        {

        }
    }
    else
    {
        QMessageBox::critical(this, "Error", "no room or user exist");
    }
}

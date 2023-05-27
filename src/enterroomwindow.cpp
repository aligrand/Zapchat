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

        emit server->command("ROOM-EXIST " + ui->id_le->text());
    }
    else {
        isGroup = false;

        emit server->command("USER-EXIST " + ui->id_le->text());
    }
}

void EnterRoomWindow::enterRoom(bool result)
{
    if (result)
    {
        if (isGroup)
        {
            emit server->command("ENTER-ROOM " + ui->id_le->text());
        }
        else
        {
            emit server->command("ROOM-USER " + ui->id_le->text());
        }
    }
    else
    {
        QMessageBox::critical(this, "Error", "no room or user exist");
    }

    this->deleteLater();
}

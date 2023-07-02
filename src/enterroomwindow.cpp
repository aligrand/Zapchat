#include "enterroomwindow.h"
#include "ui_enterroomwindow.h"

EnterRoomWindow::EnterRoomWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EnterRoomWindow)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_DeleteOnClose);

    connect(server, &ServerMan::userNameExistResult, this, &EnterRoomWindow::enterRoom);
    connect(server, &ServerMan::idExistResult, this, &EnterRoomWindow::enterRoom);
}

EnterRoomWindow::~EnterRoomWindow()
{
    emit windowClosed();
    delete ui;
}

void EnterRoomWindow::on_enterButton_clicked()
{
    QSqlQuery sqlQuery;

    if (ui->id_le->text() == "")
    {
        QMessageBox::critical(this, "Error", "You must enter ID");
    }

    if (ui->comboBox->currentText() == "Group")
    {
        isGroup = true;

        sqlQuery.prepare("SELECT COUNT(*) FROM rooms WHERE id=?");
        sqlQuery.addBindValue(ui->id_le->text());
        sqlQuery.exec();
        sqlQuery.first();
        int sqlsize = sqlQuery.value("COUNT(*)").toInt();

        if (sqlsize != 0)
        {
            QMessageBox::critical(this, "Error", "You had this group already");
            return;
        }

        emit server->command("ROOM-EXIST " + ui->id_le->text());
    }
    else
    {
        isGroup = false;

        sqlQuery.prepare("SELECT COUNT(id) FROM rooms INNER JOIN participants ON "
                         "participants.roomID=rooms.id"
                         "WHERE rooms.type=0 AND participants.userID=?");
        sqlQuery.addBindValue(ui->id_le->text());
        sqlQuery.exec();
        sqlQuery.first();
        int sqlsize = sqlQuery.value("COUNT(id)").toInt();

        if (sqlsize != 0)
        {
            QMessageBox::critical(this, "Error", "You had this contact already");
            return;
        }

        emit server->command("UN-EXIST " + ui->id_le->text());
    }
}

void EnterRoomWindow::enterRoom(bool result, QString un_id)
{
    if (un_id == ui->id_le->text())
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
}


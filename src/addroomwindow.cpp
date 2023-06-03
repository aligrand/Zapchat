#include "addroomwindow.h"
#include "ui_addroomwindow.h"

AddRoomWindow::AddRoomWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AddRoomWindow)
{
    ui->setupUi(this);

    connect(server, &ServerMan::idExistResult, this, &AddRoomWindow::addRoom);
}

AddRoomWindow::~AddRoomWindow()
{
    emit windowClosed();
    delete ui;
}

void AddRoomWindow::on_profile_pic_button_clicked()
{
    image_path = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "./", tr("Image Files (*.png *.jpg)"));

    if (image_path == "")
    {
        QMessageBox::warning(this, "Warning", "No file selected");

        return;
    }

    QPixmap pixmap(image_path);
    ui->profile_pic->setPixmap(pixmap);
}

void AddRoomWindow::on_goto_chat_button_clicked()
{
    emit server->command("ROOM-EXIST " + ui->id_le->text());
}

void AddRoomWindow::addRoom(bool id_exist, QString id)
{
    if (id == ui->id_le->text())
    {
        QSqlQuery sqlQuery;
        SqlRecordQString sqlRecS;

        if(id_exist == false)
        {
            if (name == "")
            {
                QMessageBox::critical(this, "Error", "You must enter Name");

                return;
            }

            id = ui->id_le->text();
            name = ui->name_le->text();
            info = ui->info_te->toPlainText();

            if (image_path != "")
            {
                QFile::copy(image_path, "Cache/" + id + "P." + image_path.split(".").last());
                image_path = "Cache/" + id + "P." + image_path.split(".").last();
            }

            sqlQuery.prepare("INSER INTO rooms (id, name, photoADDRESS, info, type, pin) VALUES (:id, :name,"
                             ":photoADDRESS, :info, :type, :pin)");
            sqlQuery.bindValue(":id", id);
            sqlQuery.bindValue(":name", name);
            sqlQuery.bindValue(":photoADRESS", image_path);
            sqlQuery.bindValue(":info", info);
            sqlQuery.bindValue(":type", 1);
            sqlQuery.bindValue(":pin", "");
            sqlQuery.exec();

            sqlQuery.prepare("INSERT INTO participants (userID, roomID, role) VALUES (?, ?, ?)");
            sqlQuery.addBindValue(myUsername);
            sqlQuery.addBindValue(id);
            sqlQuery.addBindValue("M");
            sqlQuery.exec();

            sqlRecS << id << name << image_path.split("/").last() << info << "1" << "";
            sqlRecS.end();
            emit server->command(QString("ADD-ROOM ") + sqlRecS);
            emit server->databaseUpdated("rooms");
        }
        else
        {
            QMessageBox::critical(this, "Error", "ID exist");
        }

        this->deleteLater();
    }
}

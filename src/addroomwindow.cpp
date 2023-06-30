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

    if (QFile::exists("Profiles/" + id + "P." + image_path.split(".").last()))
    {
        QFile::remove("Profiles/" + id + "P." + image_path.split(".").last());
    }
    QFile::copy(image_path, "Profiles/" + ui->id_le->text() + "P." + image_path.split(".").last());
    image_path = ui->id_le->text() + "P." + image_path.split(".").last();

    ui->profile_pic->setPixmap(QPixmap("Profiles/" + image_path));
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
            if (ui->name_le->text() == "")
            {
                QMessageBox::critical(this, "Error", "You must enter Name");

                return;
            }

            id = ui->id_le->text();
            name = ui->name_le->text();
            info = ui->info_te->toPlainText();

            sqlQuery.prepare("INSERT INTO rooms (id, name, photoADDRESS, info, type, pin) "
                             "VALUES (?, ?, ?, ?, ?, ?)");
            sqlQuery.addBindValue(id);
            sqlQuery.addBindValue(name);
            sqlQuery.addBindValue(image_path);
            sqlQuery.addBindValue(info);
            sqlQuery.addBindValue(1);
            sqlQuery.addBindValue("");
            sqlQuery.exec();

            sqlQuery.prepare("INSERT INTO participants (userID, roomID, role) VALUES (?, ?, ?)");
            sqlQuery.addBindValue(myUsername);
            sqlQuery.addBindValue(id);
            sqlQuery.addBindValue("M");
            sqlQuery.exec();

            sqlRecS << id << name << image_path << info << "1" << "";
            sqlRecS.end();

            if (!image_path.isEmpty())
            {
                emit server->command("_UPLOAD_ Profiles/" + image_path);
            }

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

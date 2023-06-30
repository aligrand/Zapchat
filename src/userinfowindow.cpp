#include "userinfowindow.h"
#include "ui_userinfowindow.h"

UserInfoWindow::UserInfoWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UserInfoWindow)
{
    ui->setupUi(this);
}

UserInfoWindow::~UserInfoWindow()
{
    delete ui;
}

void UserInfoWindow::on_profile_pic_button_clicked()
{
    image_path = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "./", tr("Image Files (*.png *.jpg)"));

    if (image_path == "")
    {
        QMessageBox::warning(this, "Warning", "No file selected");

        return;
    }

    if (QFile::exists("Profiles/" + myUsername + "P." + image_path.split(".").last()))
    {
        QFile::remove("Profiles/" + myUsername + "P." + image_path.split(".").last());
    }
    QFile::copy(image_path, "Profiles/" + myUsername + "P." + image_path.split(".").last());
    image_path = myUsername + "P." + image_path.split(".").last();

    ui->profile_pic->setPixmap(QPixmap("Profiles/" + image_path));
}

void UserInfoWindow::on_goto_chat_button_clicked()
{
    if (ui->name_le->text() == "")
    {
        QMessageBox::critical(this, "Error", "You must enter Name");

        return;
    }

    emit lets_go(ui->name_le->text(), ui->info_te->toPlainText(), image_path);
}

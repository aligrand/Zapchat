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

    QPixmap pixmap(image_path);
    ui->profile_pic->setPixmap(pixmap);
}

void UserInfoWindow::on_goto_chat_button_clicked()
{
    if (name == "")
    {
        QMessageBox::critical(this, "Error", "You must enter Name");

        return;
    }

    emit lets_go(name, info, image_path);
}

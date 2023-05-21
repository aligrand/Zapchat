#include "addroomwindow.h"
#include "ui_addroomwindow.h"

AddRoomWindow::AddRoomWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AddRoomWindow)
{
    ui->setupUi(this);


}

AddRoomWindow::~AddRoomWindow()
{
    delete ui;
}

void AddRoomWindow::on_profile_pic_button_clicked()
{
    image_path = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "./", tr("Image Files (*.png *.jpg)"));

    QPixmap pixmap(image_path);
    ui->profile_pic->setPixmap(pixmap);
}

void AddRoomWindow::on_goto_chat_button_clicked()
{
    if (name == "")
    {
        QMessageBox::critical(this, "Error", "You must enter Name");

        return;
    }


}

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

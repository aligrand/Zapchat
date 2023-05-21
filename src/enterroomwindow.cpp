#include "enterroomwindow.h"
#include "ui_enterroomwindow.h"

EnterRoomWindow::EnterRoomWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EnterRoomWindow)
{
    ui->setupUi(this);
}

EnterRoomWindow::~EnterRoomWindow()
{
    delete ui;
}

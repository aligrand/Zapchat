#include "pinwindow.h"
#include "ui_pinwindow.h"

PinWindow::PinWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PinWindow)
{
    ui->setupUi(this);
}

PinWindow::~PinWindow()
{
    delete ui;
}

#include "landing_page.h"
#include "ui_landing_page.h"

landing_page::landing_page(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::landing_page)
{
    ui->setupUi(this);
    QPixmap Pix("C:/Users/MostafaXD/OneDrive/Desktop/2844296.jpg");
    ui->Backgrand->setPixmap(Pix.scaled(800,600));
    ui->groupBox->setStyleSheet("QGroupBox {background-color: #ffffff;	border-radius: 5px;} QGroupBox > * {background-color: transparent;}");
    QPixmap Wel("C:/Users/MostafaXD/OneDrive/Desktop/welcome.jpg");
    ui->label->setPixmap(Wel.scaled(331,201));
}

landing_page::~landing_page()
{
    delete ui;
}

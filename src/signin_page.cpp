#include "signin_page.h"
#include "ui_signin_page.h"
#include <QPushButton>


signin_page::signin_page(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::signin_page)
{
    ui->setupUi(this);
    QPixmap Wel("C:/Users/MostafaXD/OneDrive/Desktop/test/test/Zapchat/res/img/login.png");
    ui->label->setPixmap(Wel.scaled(800,600));
    ui->groupBox_2->setStyleSheet("QGroupBox {background-color: #ffffff;	border-radius: 8px;} QGroupBox > * {background-color: transparent;}");
    ui->groupBox_3->setStyleSheet("QGroupBox {background-color: #ffffff;	border-radius: 8px;} QGroupBox > * {background-color: transparent;}");

}

signin_page::~signin_page()
{

    delete ui;
}

void signin_page::on_pushButton_7_clicked()
{
    ui->pass->setEchoMode(QLineEdit::Password);
}


void signin_page::on_pushButton_2_clicked()
{
    ui->groupBox_2->setEnabled(true);
}

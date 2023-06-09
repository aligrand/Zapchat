#include "phone_screen.h"
#include "ui_phone_screen.h"

phone_screen::phone_screen(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::phone_screen)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_DeleteOnClose);
}

void phone_screen::set_sms(QString sms)
{
    ui->auth_code_lable->setText(sms);
}

phone_screen::~phone_screen()
{
    delete ui;
}

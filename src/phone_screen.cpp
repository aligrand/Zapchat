#include "phone_screen.h"
#include "ui_phone_screen.h"

phone_screen::phone_screen(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::phone_screen)
{
    ui->setupUi(this);
}

void phone_screen::set_sms(QString sms)
{
    ui->auth_code_lable->setText(sms);
}

void phone_screen::end_window()
{
    delete this;
}

phone_screen::~phone_screen()
{
    delete ui;
}

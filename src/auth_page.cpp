#include "auth_page.h"
#include "ui_auth_page.h"

auth_page::auth_page(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::auth_page),
    ph(new phone_screen())
{
    ui->setupUi(this);
    ui->reSend_sms_button->setEnabled(false);
    timer.setInterval(100000);

    random();
}

auth_page::~auth_page()
{
    delete ui;
}

void auth_page::on_reSend_sms_button_clicked()
{
    ph->end_window();
    ph = new phone_screen();

    ph->set_sms(code_gen());

    timer.setInterval(100000);

    ph->show();

    ui->reSend_sms_button->setEnabled(false);
}

QString auth_page::code_gen()
{
    int rand = random.bounded(0,9999);
    code.number(rand);

    return code;
}

uByte auth_page::start_process() // timer + process
{
    uByte min, sec;

    ph->set_sms(code_gen());
    ph->show();

    timer.start();

    while (true)
    {
        sec = timer.remainingTime() % 60;
        min = timer.remainingTime() / 60;

        ui->remain_time_lable->setText(QString::number(min) + ":" + QString::number(sec));

        if(timer.remainingTime() <= 0)
        {
            ui->reSend_sms_button->setEnabled(true);
        }

        if(status != 0)
        {
            return status;
        }
    }
}

void auth_page::on_verify_button_clicked()
{
    QString user_input = ui->verify_dig1->text() + ui->verify_dig2->text()
            + ui->verify_dig3->text() + ui->verify_dig4->text();

    if(user_input == code)
    {
        status = 1;
    }
    else
    {
        QMessageBox::critical(this,"Zapchat","You enter wrone code");
    }
}

void auth_page::on_back2info_link_linkActivated(const QString &link)
{
    status = 2;
}

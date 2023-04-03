#include "auth_page.h"
#include "ui_auth_page.h"

auth_page::auth_page(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::auth_page),
    ph(new phone_screen()),
    timer(new QTimer(this))
{
    ui->setupUi(this);

    ui->reSend_sms_button->setEnabled(false);

    ui->back2info_link->setText("<a style=\"text-decoration:none; color:rgb(118, 118, 118);\"; href=\"#\">Is your informations wrong?</a>");


    connect(timer, &QTimer::timeout, this, &auth_page::auth_page_timer);
    timer->setTimerType(Qt::PreciseTimer);
    timer->setInterval(1000);

    connect(this, &auth_page::send_sms_code, this, &auth_page::on_reSend_sms_button_clicked);

    srand(time((time_t *)nullptr));
}

auth_page::~auth_page()
{
    delete ui;
    delete timer;
    delete ph;
}

void auth_page::closeEvent(QCloseEvent *event)
{
    delete ph;

    QWidget::closeEvent(event);
}

void auth_page::showEvent(QShowEvent *event)
{
    emit send_sms_code();
}

void auth_page::auth_page_timer()
{
    int sec = timer->remainingTime() % 60;
    int min = timer->remainingTime() / 60;

    ui->Timer_lable->setText(QString::number(min) + ":" + QString::number(sec));
}

void auth_page::on_reSend_sms_button_clicked()
{
    ph->end_window();
    ph = new phone_screen();

    ph->set_sms(code_gen());

    timer->start(100000); // 100 SECs

    ph->show();

    ui->reSend_sms_button->setEnabled(false);
}

QString auth_page::code_gen()
{
    code = QString::number(random(1000, 9999));

    return code;
}

void auth_page::on_verify_button_clicked()
{
    QString user_input = ui->verify_dig1->text() + ui->verify_dig2->text()
            + ui->verify_dig3->text() + ui->verify_dig4->text();

    if(user_input == code)
    {
        emit result_ready(1);
    }
    else
    {
        QMessageBox::critical(this,"Zapchat","You enter wrone code");
    }
}

void auth_page::on_back2info_link_linkActivated(const QString &link)
{
    emit result_ready(2);
}

inline int auth_page::random(int min, int max)
{
    return (rand() % (max - min + 1)) + min;
}



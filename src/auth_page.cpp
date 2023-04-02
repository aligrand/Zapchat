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

    connect(timer, &QTimer::timeout, this, &auth_page::auth_page_timer);
    timer->setTimerType(Qt::PreciseTimer);
    timer->setInterval(1000);

    random();
}

auth_page::~auth_page()
{
    delete ui;
    delete timer;
}

void auth_page::closeEvent(QCloseEvent *event)
{
    delete ph;

    QWidget::closeEvent(event);
}

void auth_page::showEvent(QShowEvent *event)
{
    timer->start(100000); // 100 SECs
    ph->show();
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
    int rand = random.bounded(0,9999);
    code.number(rand);

    return code;
}

void auth_page::on_verify_button_clicked()
{
    QString user_input = ui->verify_dig1->text() + ui->verify_dig2->text()
            + ui->verify_dig3->text() + ui->verify_dig4->text();

    if(user_input == code)
    {
        result_ready(1);
    }
    else
    {
        QMessageBox::critical(this,"Zapchat","You enter wrone code");
    }
}

void auth_page::on_back2info_link_linkActivated(const QString &link)
{
    result_ready(2);
}



#include "signiu_page.h"
#include "ui_signiu_page.h"

signIU_page::signIU_page(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::signIU_page),
    ap_window(new auth_page())
{
    ui->setupUi(this);

    si_w = new signin_widget(this);
    si_w->setVisible(false);
    si_w->move(293, 85);

    su_w = new signup_widget(this);
    su_w->setVisible(false);
    su_w->move(163, 30);

    si_w->setVisible(true);

    connect(ap_window, &auth_page::result_ready, this, &signIU_page::auth_result);

    connect(si_w, &signin_widget::goto_su, this, &signIU_page::goto_signup);
    connect(si_w, &signin_widget::si, this, &signIU_page::signin);
    connect(su_w, &signup_widget::goto_si, this, &signIU_page::goto_signin);
    connect(su_w, &signup_widget::su, this, &signIU_page::signup);
    connect(su_w, &signup_widget::is_username_avalable, this, &signIU_page::is_username_avalable);
}

signIU_page::~signIU_page()
{

    delete ui;
    delete si_w;
    delete su_w;
}

void signIU_page::goto_signin()
{
    su_w->setVisible(false);

    si_w->setVisible(true);
}

void signIU_page::goto_signup()
{
    si_w->setVisible(false);

    su_w->setVisible(true);
}

void signIU_page::signin(QString username, QString password)
{

}

void signIU_page::signup(QString username, QString password, QString pn, QString email)
{
    su_info[0] = username;
    su_info[1] = password;
    su_info[2] = pn;
    su_info[3] = email;

    this->hide();
    ap_window->show();
}

void signIU_page::auth_result(uByte res)
{
    // goto signup server
    if (res == 1) {
        signup_server(su_info[0], su_info[1], su_info[2], su_info[3]);
        delete ap_window;
    }
    // back to signIU
    else {
        ap_window->hide();
        this->show();
    }
}

void signIU_page::is_username_avalable(QString username, bool &result)
{
    result = true;
}

void signIU_page::signup_server(QString username, QString password, QString pn, QString email)
{

}

bool is_username_avalable(QString username)
{

}

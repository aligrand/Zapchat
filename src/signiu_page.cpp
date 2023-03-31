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

    connect(si_w, &signin_widget::goto_su, this, &signIU_page::goto_signup);
    connect(su_w, &signup_widget::goto_si, this, &signIU_page::goto_signin);
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

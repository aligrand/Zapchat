#include "signiu_page.h"
#include "ui_signiu_page.h"

signIU_page::signIU_page(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::signIU_page)
{
    ui->setupUi(this);
}

signIU_page::~signIU_page()
{
    delete ui;
}

void signIU_page::on_pushButton_clicked()
{
    ap_window = new auth_page;
    this->setVisible(false);
    ap_window->show();
    int stat = ap_window->start_process();
    if(stat == 1)
    {
        delete this;
    }
    delete ap_window;

    this->setVisible(true);
}

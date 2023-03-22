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

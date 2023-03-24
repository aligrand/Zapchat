#include "landing_page.h"
#include "ui_landing_page.h"

landing_page::landing_page(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::landing_page)
{
    ui->setupUi(this);
}

landing_page::~landing_page()
{
    delete ui;
}

void landing_page::on_goto_signiu_button_clicked()
{
    this->go_next_window = true;
    this->close();
}

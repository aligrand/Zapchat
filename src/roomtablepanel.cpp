#include "roomtablepanel.h"
#include "ui_roomtablepanel.h"

RoomTablePanel::RoomTablePanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RoomTablePanel)
{
    ui->setupUi(this);
}

RoomTablePanel::~RoomTablePanel()
{
    delete ui;
}

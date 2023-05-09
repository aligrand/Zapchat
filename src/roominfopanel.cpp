#include "roominfopanel.h"
#include "ui_roominfopanel.h"

RoomInfoPanel::RoomInfoPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RoomInfoPanel)
{
    ui->setupUi(this);
}

RoomInfoPanel::~RoomInfoPanel()
{
    delete ui;
}

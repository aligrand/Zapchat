#include "roomlistpanel.h"
#include "ui_roomlistpanel.h"

RoomListPanel::RoomListPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RoomListPanel)
{
    ui->setupUi(this);
}

RoomListPanel::~RoomListPanel()
{
    delete ui;
}

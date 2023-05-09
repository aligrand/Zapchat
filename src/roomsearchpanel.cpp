#include "roomsearchpanel.h"
#include "ui_roomsearchpanel.h"

RoomSearchPanel::RoomSearchPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RoomSearchPanel)
{
    ui->setupUi(this);
}

RoomSearchPanel::~RoomSearchPanel()
{
    delete ui;
}

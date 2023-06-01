#include "roomsdisplayer.h"
#include "ui_roomsdisplayer.h"

RoomsDisplayer::RoomsDisplayer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RoomsDisplayer)
{
    ui->setupUi(this);
}

RoomsDisplayer::~RoomsDisplayer()
{
    delete ui;
}

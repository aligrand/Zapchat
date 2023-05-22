#include "roomwidget.h"
#include "ui_roomwidget.h"

RoomWidget::RoomWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RoomWidget)
{
    ui->setupUi(this);
}

RoomWidget::~RoomWidget()
{
    delete ui;
}

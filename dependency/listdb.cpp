#include "listdb.h"
#include "ui_listdb.h"

ListDB::ListDB(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListDB)
{
    ui->setupUi(this);
}

ListDB::~ListDB()
{
    delete ui;
}

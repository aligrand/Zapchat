#include "appsettingspanel.h"
#include "ui_appsettingspanel.h"

AppSettingsPanel::AppSettingsPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AppSettingsPanel)
{
    ui->setupUi(this);
}

AppSettingsPanel::~AppSettingsPanel()
{
    delete ui;
}

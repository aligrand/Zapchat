#include "accountsettingspanel.h"
#include "ui_accountsettingspanel.h"

AccountSettingsPanel::AccountSettingsPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AccountSettingsPanel)
{
    ui->setupUi(this);
}

AccountSettingsPanel::~AccountSettingsPanel()
{
    delete ui;
}

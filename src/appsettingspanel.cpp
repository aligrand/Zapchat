#include "appsettingspanel.h"
#include "ui_appsettingspanel.h"

AppSettingsPanel::AppSettingsPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AppSettingsPanel)
{
    ui->setupUi(this);

    ui->BGLable->setPixmap(QPixmap(iniFile["chat-bg-image"]));
}

AppSettingsPanel::~AppSettingsPanel()
{
    delete ui;
}

void AppSettingsPanel::on_pushButton_clicked()
{
    image_path = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "./background-image", tr("Image Files (*.png *.jpg)"));

    if (image_path == "")
    {
        QMessageBox::warning(this, "Warning", "No file selected");

        return;
    }

    QPixmap pixmap(image_path);
    ui->BGLable->setPixmap(pixmap);
}

void AppSettingsPanel::on_okButton_clicked()
{
    iniFile.write("chat-bg-image", image_path);

    emit preferencesUpdated();
}

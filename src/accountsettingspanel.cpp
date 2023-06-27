#include "accountsettingspanel.h"
#include "ui_accountsettingspanel.h"

AccountSettingsPanel::AccountSettingsPanel(bool isUser, QString ID, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AccountSettingsPanel)
{
    ui->setupUi(this);

    is_user = isUser;
    id = ID;

    QSqlQuery sqlQuery;

    if (isUser)
    {
        sqlQuery.prepare("SELECT * FROM users WHERE username=?");
        sqlQuery.addBindValue(ID);
        sqlQuery.exec();
        sqlQuery.first();

        ui->nameField->setText(sqlQuery.value("name").toString());
        ui->infoField->setText(sqlQuery.value("info").toString());

        if (!sqlQuery.value("photoADDRESS").toString().isEmpty())
        {
            ui->profilePic->setPixmap(QPixmap("Profiles/" + sqlQuery.value("photoADDRESS").toString()));
        }

        ui->pnField->setText(sqlQuery.value("phoneNumber").toString());
        ui->emailField->setText(sqlQuery.value("emailAddress").toString());
    }
    else
    {
        delete ui->pnField;
        delete ui->emailField;

        sqlQuery.prepare("SELECT * FROM rooms WHERE id=?");
        sqlQuery.addBindValue(ID);
        sqlQuery.exec();
        sqlQuery.first();

        ui->nameField->setText(sqlQuery.value("name").toString());
        ui->infoField->setText(sqlQuery.value("info").toString());

        if (!sqlQuery.value("photoADDRESS").toString().isEmpty())
        {
            ui->profilePic->setPixmap(QPixmap("Profiles/" + sqlQuery.value("photoADDRESS").toString()));
        }
    }
}

AccountSettingsPanel::~AccountSettingsPanel()
{
    delete ui;
}

void AccountSettingsPanel::on_picSelectButton_clicked()
{
    image_path = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "./", tr("Image Files (*.png *.jpg)"));

    if (image_path == "")
    {
        QMessageBox::warning(this, "Warning", "No file selected");

        return;
    }

    QFile::copy(image_path, "Profiles/" + myUsername + "P." + image_path.split(".").last());
    image_path = myUsername + "P." + image_path.split(".").last();

    ui->profilePic->setPixmap(QPixmap("Profiles/" + image_path));
}

void AccountSettingsPanel::on_okButton_clicked()
{
    SqlRecordQString record;
    QSqlQuery sqlQuery;

    if (server->getNetworkState() == NetworkState::Offline)
    {
        QMessageBox::critical(this, "Error", "You are not connected");

        return;
    }

    if (!image_path.isEmpty())
    {
        emit server->command("_UPLOAD_ Profiles/" + image_path);
    }

    if (is_user)
    {
        sqlQuery.prepare("SELECT * FROM users WHERE username=?");
        sqlQuery.addBindValue(id);
        sqlQuery.exec();
        sqlQuery.first();

        record << sqlQuery.value("username").toString() << ui->emailField->text() << ui->pnField->text()
               << ui->nameField->text() << image_path << ui->infoField->toPlainText()
               << QString(server->getNetworkState());
        record.end();

        emit server->command(QString("EDIT-USER ") + record);
    }
    else
    {
         sqlQuery.prepare("SELECT * FROM rooms WHERE id=?");
         sqlQuery.addBindValue(id);
         sqlQuery.exec();
         sqlQuery.first();

         record << sqlQuery.value("id").toString() << ui->nameField->text() << image_path
                << ui->infoField->toPlainText() << sqlQuery.value("type").toString()
                << sqlQuery.value("pin").toString();
         record.end();

         emit server->command(QString("EDIT-ROOM ") + record);
    }
}

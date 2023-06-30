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
            image_path = sqlQuery.value("photoADDRESS").toString();
        }

        ui->pnField->setText(sqlQuery.value("phoneNumber").toString());
        ui->emailField->setText(sqlQuery.value("emailAddress").toString());
    }
    else
    {
        delete ui->groupBox_3;
        delete ui->groupBox_4;

        sqlQuery.prepare("SELECT * FROM rooms WHERE id=?");
        sqlQuery.addBindValue(ID);
        sqlQuery.exec();
        sqlQuery.first();

        ui->nameField->setText(sqlQuery.value("name").toString());
        ui->infoField->setText(sqlQuery.value("info").toString());

        if (!sqlQuery.value("photoADDRESS").toString().isEmpty())
        {
            ui->profilePic->setPixmap(QPixmap("Profiles/" + sqlQuery.value("photoADDRESS").toString()));
            image_path = sqlQuery.value("photoADDRESS").toString();
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

    if (QFile::exists("Profiles/" + id + "P." + image_path.split(".").last()))
    {
        QFile::remove("Profiles/" + id + "P." + image_path.split(".").last());
    }
    QFile::copy(image_path, "Profiles/" + id + "P." + image_path.split(".").last());
    image_path = id + "P." + image_path.split(".").last();

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
        record << id << ui->emailField->text() << ui->pnField->text()
               << ui->nameField->text() << image_path << ui->infoField->toPlainText()
               << "1";
        record.end();

        emit server->command(QString("EDIT-USER ") + record);

        sqlQuery.prepare("UPDATE users SET emailAddress=?, phoneNumber=?, name=?, "
                         "photoADDRESS=?, info=?, isOnline=? WHERE username=?");
        sqlQuery.addBindValue(ui->emailField->text());
        sqlQuery.addBindValue(ui->pnField->text());
        sqlQuery.addBindValue(ui->nameField->text());
        sqlQuery.addBindValue(image_path);
        sqlQuery.addBindValue(ui->infoField->toPlainText());
        sqlQuery.addBindValue(1);
        sqlQuery.addBindValue(id);
        sqlQuery.exec();
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

#ifndef ACCOUNTSETTINGSPANEL_H
#define ACCOUNTSETTINGSPANEL_H

#include <QWidget>
#include <QString>
#include <QSqlQuery>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>

#include "lib/server-connection/serverman.h"
#include "lib/server-connection/sqlrecordqstring.h"

extern ServerMan *server;
extern QString myUsername;

namespace Ui {
class AccountSettingsPanel;
}

class AccountSettingsPanel : public QWidget
{
    Q_OBJECT

public:
    explicit AccountSettingsPanel(bool isUser, QString ID, QWidget *parent = nullptr);
    ~AccountSettingsPanel();

private slots:
    void on_picSelectButton_clicked();

    void on_okButton_clicked();

private:
    Ui::AccountSettingsPanel *ui;
    QString image_path= "", id = "";
    bool is_user;
};

#endif // ACCOUNTSETTINGSPANEL_H

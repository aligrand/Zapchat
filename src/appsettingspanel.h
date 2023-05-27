#ifndef APPSETTINGSPANEL_H
#define APPSETTINGSPANEL_H

#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>

#include "lib/ini-proc/iniproc.h"

namespace Ui {
class AppSettingsPanel;
}

class AppSettingsPanel : public QWidget
{
    Q_OBJECT

public:
    explicit AppSettingsPanel(QWidget *parent = nullptr);
    ~AppSettingsPanel();

signals:
    void preferencesUpdated();

private slots:
    void on_pushButton_clicked();

    void on_okButton_clicked();

private:
    Ui::AppSettingsPanel *ui;
    QString image_path = "";
    IniProc iniFile = IniProc("settings.ini");
};

#endif // APPSETTINGSPANEL_H

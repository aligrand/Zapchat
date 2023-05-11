#ifndef APPSETTINGSPANEL_H
#define APPSETTINGSPANEL_H

#include <QWidget>

namespace Ui {
class AppSettingsPanel;
}

class AppSettingsPanel : public QWidget
{
    Q_OBJECT

public:
    explicit AppSettingsPanel(QWidget *parent = nullptr);
    ~AppSettingsPanel();

private:
    Ui::AppSettingsPanel *ui;
};

#endif // APPSETTINGSPANEL_H

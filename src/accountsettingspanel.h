#ifndef ACCOUNTSETTINGSPANEL_H
#define ACCOUNTSETTINGSPANEL_H

#include <QWidget>

namespace Ui {
class AccountSettingsPanel;
}

class AccountSettingsPanel : public QWidget
{
    Q_OBJECT

public:
    explicit AccountSettingsPanel(QWidget *parent = nullptr);
    ~AccountSettingsPanel();

private:
    Ui::AccountSettingsPanel *ui;
};

#endif // ACCOUNTSETTINGSPANEL_H

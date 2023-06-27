#ifndef PHONE_SCREEN_H
#define PHONE_SCREEN_H

#include <QMainWindow>

namespace Ui {
class phone_screen;
}

class phone_screen : public QMainWindow
{
    Q_OBJECT

public:
    explicit phone_screen(QWidget *parent = nullptr);
    ~phone_screen();
    void set_sms(QString sms);

private:
    Ui::phone_screen *ui;
};

#endif // PHONE_SCREEN_H

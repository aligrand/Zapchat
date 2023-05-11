#ifndef PINWINDOW_H
#define PINWINDOW_H

#include <QMainWindow>

namespace Ui {
class PinWindow;
}

class PinWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PinWindow(QWidget *parent = nullptr);
    ~PinWindow();

private:
    Ui::PinWindow *ui;
};

#endif // PINWINDOW_H

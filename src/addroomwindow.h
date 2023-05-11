#ifndef ADDROOMWINDOW_H
#define ADDROOMWINDOW_H

#include <QMainWindow>

namespace Ui {
class AddRoomWindow;
}

class AddRoomWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AddRoomWindow(QWidget *parent = nullptr);
    ~AddRoomWindow();

private:
    Ui::AddRoomWindow *ui;
};

#endif // ADDROOMWINDOW_H

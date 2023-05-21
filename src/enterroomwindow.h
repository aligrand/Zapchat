#ifndef ENTERROOMWINDOW_H
#define ENTERROOMWINDOW_H

#include <QMainWindow>

namespace Ui {
class EnterRoomWindow;
}

class EnterRoomWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit EnterRoomWindow(QWidget *parent = nullptr);
    ~EnterRoomWindow();

private:
    Ui::EnterRoomWindow *ui;
};

#endif // ENTERROOMWINDOW_H

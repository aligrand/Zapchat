#ifndef ENTERROOMWINDOW_H
#define ENTERROOMWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

#include "lib/server-connection/serverman.h"

extern ServerMan server;

namespace Ui {
class EnterRoomWindow;
}

class EnterRoomWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit EnterRoomWindow(QWidget *parent = nullptr);
    ~EnterRoomWindow();

private slots:
    void on_goto_chat_button_clicked();

    void enterRoom(bool result);

private:
    Ui::EnterRoomWindow *ui;
};

#endif // ENTERROOMWINDOW_H

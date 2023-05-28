#ifndef ENTERROOMWINDOW_H
#define ENTERROOMWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QSqlQuery>

#include "lib/server-connection/serverman.h"
#include "lib/server-connection/sqlrecordqstring.h"

extern ServerMan *server;
extern QString myUsername;

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

    void enterRoom(bool result, QString un_id);

private:
    Ui::EnterRoomWindow *ui;
    bool isGroup;
};

#endif // ENTERROOMWINDOW_H

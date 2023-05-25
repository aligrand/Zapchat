#ifndef ADDROOMWINDOW_H
#define ADDROOMWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QSqlQuery>
#include <QStringList>

#include "lib/server-connection/serverman.h"
#include "lib/server-connection/sqlrecordqstring.h"

extern ServerMan *server;
extern QString myUsername;

namespace Ui {
class AddRoomWindow;
}

class AddRoomWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AddRoomWindow(QWidget *parent = nullptr);
    ~AddRoomWindow();

signals:
    void roomCreated();

private slots:
    void on_profile_pic_button_clicked();

    void on_goto_chat_button_clicked();

    void addRoom(bool id_exist);


private:
    Ui::AddRoomWindow *ui;
    QString name = "", info = "", image_path= "", id = "";
};

#endif // ADDROOMWINDOW_H

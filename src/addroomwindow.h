#ifndef ADDROOMWINDOW_H
#define ADDROOMWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QFileDialog>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>

namespace Ui {
class AddRoomWindow;
}

class AddRoomWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AddRoomWindow(QWidget *parent = nullptr);
    ~AddRoomWindow();

private slots:
    void on_profile_pic_button_clicked();

    void on_goto_chat_button_clicked();

private:
    Ui::AddRoomWindow *ui;
    QString name = "", info = "", image_path= "";
    QSqlDatabase db;
};

#endif // ADDROOMWINDOW_H

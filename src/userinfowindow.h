#ifndef USERINFOWINDOW_H
#define USERINFOWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QPixmap>
#include <QMessageBox>
#include <QFileDialog>

extern QString myUsername;

namespace Ui {
class UserInfoWindow;
}

class UserInfoWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit UserInfoWindow(QWidget *parent = nullptr);
    ~UserInfoWindow();

signals:
    void lets_go(QString name, QString info, QString image_path);

private slots:
    void on_profile_pic_button_clicked();

    void on_goto_chat_button_clicked();

private:
    Ui::UserInfoWindow *ui;
    QString image_path = "";

    void set_profile_img();
};

#endif // USERINFOWINDOW_H

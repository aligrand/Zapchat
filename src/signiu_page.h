#ifndef SIGNIU_PAGE_H
#define SIGNIU_PAGE_H

#include "auth_page.h"
#include "signin_widget.h"
#include "signup_widget.h"
#include "userinfowindow.h"

#include <QMainWindow>
#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QTextStream>

typedef unsigned char uByte;

namespace Ui {
class signIU_page;
}

class signIU_page : public QMainWindow
{
    Q_OBJECT

public:
    explicit signIU_page(QWidget *parent = nullptr);
    ~signIU_page();

    bool go_next_window = false;

private slots:
    void goto_signin();
    void goto_signup();
    void signin(QString username, QString password, bool &result);
    void signup(QString username, QString password, QString pn, QString email);
    void auth_result(uByte res);
    void is_username_exist(QString username, bool &result);

private:
    Ui::signIU_page *ui;
    auth_page *ap_window;
    signin_widget *si_w;
    signup_widget *su_w;
    UserInfoWindow *userinfo_w;
    QString su_info[4];
    QJsonDocument database;
    QFile database_file;

    void signup_server(QString username, QString password, QString pn, QString email);
};

#endif // SIGNIU_PAGE_H

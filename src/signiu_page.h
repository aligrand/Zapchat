#ifndef SIGNIU_PAGE_H
#define SIGNIU_PAGE_H

#include "auth_page.h"
#include "signin_widget.h"
#include "signup_widget.h"
#include "userinfowindow.h"

#include <QMainWindow>
#include <QString>
#include <QFile>
#include <QMessageBox>
#include <QSqlQuery>

#include "lib/server-connection/serverman.h"
#include "lib/server-connection/sqlrecordqstring.h"

extern ServerMan *server;
extern QString myUsername;

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
    void signin(QString username, QString password);
    void signin_result(int result);
    void signup(QString username, QString password, QString pn, QString email);
    void auth_result(qint8 res);
    void uiw_result(QString name, QString info, QString image_path);
    void closeByServer();

private:
    Ui::signIU_page *ui;
    auth_page *ap_window;
    signin_widget *si_w;
    signup_widget *su_w;
    UserInfoWindow *userinfo_w;
    QString sign_info[4];
    QString sign_way = "";

    void signup_server(QString username, QString password, QString pn, QString email);
};

#endif // SIGNIU_PAGE_H

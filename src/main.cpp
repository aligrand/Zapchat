#include "landing_page.h"
#include "signiu_page.h"
#include "userinfowindow.h"
#include "chatwindow.h"

#include "lib/server-connection/serverman.h"

#include <QApplication>
#include <QFile>
#include <QSqlDatabase>

using namespace std;

ServerMan *server;
QString myUsername;

bool is_user_avalable()
{
    QFile userinfo_file("./userinfo.txt");
    qint64 size = userinfo_file.size();
    userinfo_file.close();

    if(size == 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    landing_page *lp_window = new landing_page();
    signIU_page *sp_window = new signIU_page();
    ChatWindow *cw = new ChatWindow();

    QString password = "";

    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("database.sqlite3");
    db.open();

    QFile userpassFile("userinfo.txt");
    userpassFile.open(QIODevice::ReadOnly | QIODevice::Text);
    myUsername = userpassFile.readLine().trimmed();
    password = userpassFile.readLine().trimmed();
    userpassFile.close();

    server = new ServerMan();

    if(!is_user_avalable())
    {
        lp_window->show();

        while (!lp_window->go_next_window)
        {
            a.processEvents();
        }

        delete lp_window;

        sp_window->show();

        while (!sp_window->go_next_window)
        {
            a.processEvents();
        }

        delete sp_window;
    }

    cw->show();

    int exec_ret = a.exec();

    db.close();

    return exec_ret;
}

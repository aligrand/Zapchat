#include "landing_page.h"
#include "signiu_page.h"
#include "userinfowindow.h"
#include "chatwindow.h"

#include "lib/server-connection/serverman.h"

#include <QApplication>
#include <QFile>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QTime>

#include <assert.h>

ServerMan *server;
QString myUsername;
qint64 myMessageIndex;

// min : 200 milisec
void delay(int milisec)
{
    assert(milisec >= 200);

    QTime dieTime= QTime::currentTime().addMSecs(milisec);
    while (QTime::currentTime() < dieTime)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}

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
    landing_page *lp_window;
    signIU_page *sp_window;
    ChatWindow *cw;

    QString password = "";

    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("clientDB.sqlite3");
    db.open();

    QSqlQuery sqlQuery;
    sqlQuery.prepare("PRAGMA foreign_keys = 1;");
    sqlQuery.exec();

    QFile userpassFile("userinfo.txt");
    userpassFile.open(QIODevice::ReadOnly | QIODevice::Text);

    server = new ServerMan;

    delay(2000);

    if(!is_user_avalable())
    {
        if (server->getNetworkState() == NetworkState::Offline)
        {
            QMessageBox::critical(nullptr, "Erorr", "Youu are offline");

            qApp->exit(0);
        }

        lp_window = new landing_page;
        lp_window->show();

        while (!lp_window->go_next_window)
        {
            a.processEvents();
        }

        delete lp_window;

        sp_window = new signIU_page;
        sp_window->show();

        while (!sp_window->go_next_window)
        {
            a.processEvents();
        }

        delete sp_window;
    }
    else
    {
        myUsername = userpassFile.readLine().trimmed();
        password = userpassFile.readLine().trimmed();
        userpassFile.close();

        if (server->getNetworkState() == NetworkState::Online)
        {
            emit server->command("LOGIN " + myUsername + " " + password);
        }
    }

    QFile mIndex("message-index.txt");
    mIndex.open(QIODevice::ReadWrite | QIODevice::Text);
    myMessageIndex = mIndex.readLine().toLongLong();

    cw = new ChatWindow;
    cw->show();

    int exec_ret = a.exec();

    db.close();

    mIndex.resize(0);
    mIndex.write(QString::number(myMessageIndex).toStdString().c_str());
    mIndex.close();

    delete server;

    return exec_ret;
}

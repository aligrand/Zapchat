#include "landing_page.h"
#include "signiu_page.h"

#include <QApplication>
#include <QDebug>
#include <QFile>
#include <cstdlib>

using namespace std;

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
    system("mkdir Audios, Chats, Files, Images, Videos");
    system("if not exist server-config.ini type nul > server-config.ini");
    system("if not exist userinfo.txt type nul > userinfo.txt");

    QApplication a(argc, argv);
    landing_page *lp_window = new landing_page();
    signIU_page *sp_window = new signIU_page();

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

    return a.exec();
}

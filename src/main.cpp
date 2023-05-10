#include "landing_page.h"
#include "signiu_page.h"
#include "userinfowindow.h"
#include "chatwindow.h"

#include <QApplication>
#include <QFile>

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
    QApplication a(argc, argv);
    landing_page *lp_window = new landing_page();
    signIU_page *sp_window = new signIU_page();
    ChatWindow *cw = new ChatWindow();

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

    return a.exec();
}

#include "landing_page.h"
#include "signiu_page.h"

#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QDir>

using namespace std;

void mk_prerequisite()
{
    static QString path[7] = {"./Audios", "./Chats", "./Files", "./Images", "./Videos",
                            "./", "./"};
    static QString file_name[7] = {".nomedia", ".nomedia", ".nomedia", ".nomedia", ".nomedia",
                                  "server-config.ini", "userinfo.txt"};

    for (qint8 i = 0; i < 7; ++i)
    {
        QDir dir;
        QFile file(path[i] + "/" + file_name[i]);

        if (!dir.exists(path[i])){
            dir.mkpath(path[i]);
        }

        if (!file.exists()){
            file.open(QIODevice::WriteOnly);
            file.close();
        }
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
    mk_prerequisite();

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

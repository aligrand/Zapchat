#include "landing_page.h"
#include "signiu_page.h"

#include <QApplication>
#include <iostream>
#include <fstream>
#include <QThread>

using namespace std;

bool is_user_avalable()
{
    ifstream userinfo_file;

    userinfo_file.open("./userinfo.dat");

    if(!userinfo_file)
    {
        cout << "error in opening/creating file";
        exit(0);
    }

    userinfo_file.seekg(0, ios::end);

    long long curser_pos = userinfo_file.tellg();
    if(curser_pos == 0)
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
    landing_page lp_window;
    signIU_page sp_window;

    if(!is_user_avalable())
    {
        lp_window.show();

        while (!lp_window.go_next_window)
        {
            a.processEvents();
        }

        sp_window.show();
    }

    return a.exec();
}

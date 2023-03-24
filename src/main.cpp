#include "landing_page.h"

#include <QApplication>
#include <fstream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);



    landing_page w;
    w.show();

    return a.exec();
}

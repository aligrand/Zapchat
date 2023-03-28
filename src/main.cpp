#include "landing_page.h"

#include <QApplication>
#include <QPushButton>

class MyFilter : public QObject
{
    Q_OBJECT
public:
    explicit MyFilter(QObject *parent = nullptr) : QObject(parent) {}

protected:

    };
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    landing_page w;
    w.show();


    return a.exec();
}

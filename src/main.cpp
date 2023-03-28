#include "landing_page.h"

#include <QApplication>
#include <QPushButton>
#include <QEvent>
#include <QDebug>
class MyFilter : public QObject
{
    Q_OBJECT
public:
    explicit MyFilter(QObject *parent = nullptr) : QObject(parent) {}

protected:
    bool eventFilter(QObject *obj, QEvent *event) override {
        if (obj == myButton && event->type() == QEvent::Enter) {
            qDebug() << "Mouse entered the button!";
        } else if (obj == myButton && event->type() == QEvent::Leave) {
            qDebug() << "Mouse left the button!";
        }
        return QObject::eventFilter(obj, event);
    }
};
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    landing_page w;
    w.show();
    QPushButton myButton("My Button");
      myButton.show();

      MyFilter *myFilter = new MyFilter(&myButton);
      myButton.installEventFilter(myFilter);
    return a.exec();
}

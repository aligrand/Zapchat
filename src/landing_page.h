#ifndef LANDING_PAGE_H
#define LANDING_PAGE_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class landing_page; }
QT_END_NAMESPACE

class landing_page : public QMainWindow
{
    Q_OBJECT

public:
    landing_page(QWidget *parent = nullptr);
    ~landing_page();

private:
    Ui::landing_page *ui;
};
#endif // LANDING_PAGE_H

#ifndef SIGNIU_PAGE_H
#define SIGNIU_PAGE_H

#include <QMainWindow>

namespace Ui {
class signIU_page;
}

class signIU_page : public QMainWindow
{
    Q_OBJECT

public:
    explicit signIU_page(QWidget *parent = nullptr);
    ~signIU_page();

private:
    Ui::signIU_page *ui;
};

#endif // SIGNIU_PAGE_H

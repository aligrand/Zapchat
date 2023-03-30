#ifndef SIGNIU_PAGE_H
#define SIGNIU_PAGE_H

#include "auth_page.h"

#include <QMainWindow>

typedef char Byte;
typedef unsigned char uByte;

namespace Ui {
class signIU_page;
}

class signIU_page : public QMainWindow
{
    Q_OBJECT

public:
    explicit signIU_page(QWidget *parent = nullptr);
    ~signIU_page();

public slots:
    uByte return_res(uByte res);

private slots:
    void on_pushButton_clicked();

private:
    Ui::signIU_page *ui;
    auth_page *ap_window;
};

#endif // SIGNIU_PAGE_H

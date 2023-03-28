#ifndef SIGNIN_PAGE_H
#define SIGNIN_PAGE_H

#include <QMainWindow>

namespace Ui {
class signin_page;
}
class signin_page : public QMainWindow
{
    Q_OBJECT

public:
    explicit signin_page(QWidget *parent = nullptr);
    ~signin_page();

private slots:
    void on_pushButton_7_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::signin_page *ui;
};

#endif // SIGNIN_PAGE_H

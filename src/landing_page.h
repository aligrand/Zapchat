#ifndef LANDING_PAGE_H
#define LANDING_PAGE_H

#include <QMainWindow>

#include "../lib/server-connection/serverman.h"

QT_BEGIN_NAMESPACE
namespace Ui { class landing_page; }
QT_END_NAMESPACE

class landing_page : public QMainWindow
{
    Q_OBJECT

public:
    bool go_next_window = false;

    landing_page(QWidget *parent = nullptr);
    ~landing_page();

private slots:
    void on_goto_signiu_button_clicked();
    void closeByServer();

private:
    Ui::landing_page *ui;
};
#endif // LANDING_PAGE_H

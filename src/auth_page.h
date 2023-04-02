#ifndef AUTH_PAGE_H
#define AUTH_PAGE_H

#include "phone_screen.h"

#include <QMainWindow>
#include <QWidget>
#include <QRandomGenerator>
#include <QString>
#include <QMessageBox>
#include <QTimer>
#include <QCloseEvent>

typedef char Byte;
typedef unsigned char uByte;

namespace Ui {
class auth_page;
}

class auth_page : public QMainWindow
{
    Q_OBJECT

public:
    explicit auth_page(QWidget *parent = nullptr);
    ~auth_page();

signals:
    void result_ready(uByte res);

protected:

    void closeEvent(QCloseEvent *event);

    void showEvent(QShowEvent *event);

private slots:
    void on_reSend_sms_button_clicked();

    void on_verify_button_clicked();

    void on_back2info_link_linkActivated(const QString &link);

    void auth_page_timer();

private:
    Ui::auth_page *ui;
    phone_screen *ph;
    QRandomGenerator random;
    QString code;
    QTimer *timer;

    QString code_gen();
};

#endif // AUTH_PAGE_H

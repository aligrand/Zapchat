#ifndef AUTH_PAGE_H
#define AUTH_PAGE_H

#include "phone_screen.h"

#include <QMainWindow>
#include <QRandomGenerator>
#include <QString>
#include <QMessageBox>
#include <QTimer>

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
    uByte start_process();

private slots:
    void on_reSend_sms_button_clicked();

    void on_verify_button_clicked();

    void on_back2info_link_linkActivated(const QString &link);

private:
    Ui::auth_page *ui;
    uByte status = 0; // 0:still in process --- 1:verified --- 2:change info
    phone_screen *ph;
    QRandomGenerator random;
    QString code;
    QTimer timer;

    QString code_gen();
};

#endif // AUTH_PAGE_H

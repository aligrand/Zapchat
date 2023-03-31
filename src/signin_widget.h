#ifndef SIGNIN_WIDGET_H
#define SIGNIN_WIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QIcon>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QLabel>
#include <QPixmap>

namespace Ui {
class signin_widget;
}

class signin_widget : public QWidget
{
    Q_OBJECT

public:
    explicit signin_widget(QWidget *parent = nullptr);
    ~signin_widget();

signals:
    void goto_su();

private slots:
    void on_username_lineEdit_textChanged(const QString &text);

    void on_password_lineEdit_textChanged(const QString &tex);

    void on_visible_OF_clicked();

    void on_login_pushButton_clicked();

    void on_goto_signup_link_linkActivated(const QString &link);

private:
    Ui::signin_widget *ui;
    bool is_password_visible = false;
    QRegularExpression username_regex, password_regex;
    QRegularExpressionValidator username_regex_val, password_regex_val;
    bool is_password_val = false , is_username_val = false;
};

#endif // SIGNIN_WIDGET_H

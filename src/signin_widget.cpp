#include "signin_widget.h"
#include "ui_signin_widget.h"

signin_widget::signin_widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::signin_widget)
{
    ui->setupUi(this);

    ui->goto_signup_link->setText("<a style=\"color:rgb(118, 118, 118); text-decoration:none;\" href=\"#\">Have'nt an account?</a>");

    username_regex.setPattern("^\\w{1,10}$");
    password_regex.setPattern("^[a-zA-Z0-9]*(\\$|%)[a-zA-Z0-9]*$");
    username_regex_val.setRegularExpression(username_regex);
    password_regex_val.setRegularExpression(password_regex);
}

signin_widget::~signin_widget()
{
    delete ui;
}

void signin_widget::on_username_lineEdit_textChanged(const QString &text)
{
    int pos = 0;
    QString text_tmp = text;

    if (username_regex_val.validate(text_tmp, pos) == QValidator::Acceptable){
        is_username_val = true;
        ui->username_lineEdit->setStyleSheet("border-left-color: #8bc34a;\
                                             border-left-style: solid;\
                                             border-left-width: 5px;\
                                             padding-left:3px;");
    }
    else if (text_tmp.isEmpty()) {
        is_username_val = false;
        ui->username_lineEdit->setStyleSheet("border-left-color: #fff;\
                                             border-left-style: solid;\
                                             border-left-width: 5px;\
                                             padding-left:3px;");
    }
    else {
        is_username_val = false;
        ui->username_lineEdit->setStyleSheet("border-left-color: #f44336;\
                                             border-left-style: solid;\
                                             border-left-width: 5px;\
                                             padding-left:3px;");
    }
}

void signin_widget::on_password_lineEdit_textChanged(const QString &text)
{
    int pos = 0;
    QString text_tmp = text;

    if ((password_regex_val.validate(text_tmp, pos) == QValidator::Acceptable) &&
            ((5 <= text_tmp.length()) && (text_tmp.length() <= 10))){
        is_password_val = true;
        ui->password_lineEdit->setStyleSheet("border-left-color: #8bc34a;\
                                             border-left-style: solid;\
                                             border-left-width: 5px;\
                                             padding-left:3px;");
    }
    else if (text_tmp.isEmpty()) {
        is_password_val = false;
        ui->password_lineEdit->setStyleSheet("border-left-color: #fff;\
                                             border-left-style: solid;\
                                             border-left-width: 5px;\
                                             padding-left:3px;");
    }
    else {
        is_password_val = false;
        ui->password_lineEdit->setStyleSheet("border-left-color: #f44336;\
                                             border-left-style: solid;\
                                             border-left-width: 5px;\
                                             padding-left:3px;");
    }
}

void signin_widget::on_visible_OF_clicked()
{
    if (is_password_visible) {
        ui->password_lineEdit->setEchoMode(QLineEdit::Password);
        ui->visible_OF->setIcon(QIcon("../res/icon/visibility-off"));
        is_password_visible = false;
    }
    else {
        ui->password_lineEdit->setEchoMode(QLineEdit::Normal);
        ui->visible_OF->setIcon(QIcon("../res/icon/visibility-on"));
        is_password_visible = true;
    }
}

void signin_widget::on_login_pushButton_clicked()
{

}

void signin_widget::on_goto_signup_link_linkActivated(const QString &link)
{
    emit goto_su();
}

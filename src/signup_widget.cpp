#include "signup_widget.h"
#include "ui_signup_widget.h"

signup_widget::signup_widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::signup_widget)
{
    ui->setupUi(this);

    ui->goto_signin_link->setText("<a style=\"text-decoration:none; color:rgb(118, 118, 118);\"; href=\"#\">Have an account?</a>");

    srand(time((time_t *)nullptr));

    username_regex.setPattern("\\w+");
    password_regex.setPattern("^[a-zA-Z0-9]*(\\$|%)[a-zA-Z0-9]*$");
    Cpassword_regex = password_regex;
    pn_regex.setPattern("^[1-9][0-9]{9}$");
    email_regex.setPattern("^(\\w+(\\.|-)?\\w+)+@+[a-zA-Z0-9]+\\.[a-zA-Z]+$");
    username_regex_val.setRegularExpression(username_regex);
    password_regex_val.setRegularExpression(password_regex);
    Cpassword_regex_val.setRegularExpression(Cpassword_regex);
    pn_regex_val.setRegularExpression(pn_regex);
    email_regex_val.setRegularExpression(email_regex);

    captcha_gen();
}

signup_widget::~signup_widget()
{
    delete ui;
}


void signup_widget::on_username_lineEdit_textChanged(const QString &text)
{
    int pos = 0;
    QString text_tmp = text;

    if (username_regex_val.validate(text_tmp, pos) == QValidator::Acceptable){
        bool res;
        emit is_username_avalable(ui->username_lineEdit->text(), res);
        if (res) {
            is_username_val = true;
            ui->username_lineEdit->setStyleSheet("border-left-color: #f44336;\
                                                 border-left-style: solid;\
                    border-left-width: 5px;\
                    background-color: #a5d6a7;\
            padding-left:3px;");
        }
        else {
            is_username_val = false;
            ui->username_lineEdit->setStyleSheet("border-left-color: #8bc34a;\
                                                 border-left-style: solid;\
                    border-left-width: 5px;\
                    background-color: #ef9a9a;\
            padding-left:3px;");
        }
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

void signup_widget::on_password_lineEdit_textChanged(const QString &text)
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

void signup_widget::on_c_password_lineEdit_textChanged(const QString &text)
{
    int pos = 0;
    QString text_tmp = text;

    if ((Cpassword_regex_val.validate(text_tmp, pos) == QValidator::Acceptable) &&
            ((5 <= text_tmp.length()) && (text_tmp.length() <= 10))){
        is_Cpassword_val = true;
        ui->c_password_lineEdit->setStyleSheet("border-left-color: #8bc34a;\
                                               border-left-style: solid;\
                border-left-width: 5px;\
        padding-left:3px;");
    }
    else if (text_tmp.isEmpty()) {
        is_Cpassword_val = false;
        ui->c_password_lineEdit->setStyleSheet("border-left-color: #fff;\
                                               border-left-style: solid;\
                border-left-width: 5px;\
        padding-left:3px;");
    }
    else {
        is_Cpassword_val = false;
        ui->c_password_lineEdit->setStyleSheet("border-left-color: #f44336;\
                                               border-left-style: solid;\
                border-left-width: 5px;\
        padding-left:3px;");
    }
}

void signup_widget::on_p_visible_OF_clicked()
{
    if (is_password_visible) {
        ui->password_lineEdit->setEchoMode(QLineEdit::Password);
        ui->p_visible_OF->setIcon(QIcon("../res/icon/visibility-off"));
        is_password_visible = false;
    }
    else {
        ui->password_lineEdit->setEchoMode(QLineEdit::Normal);
        ui->p_visible_OF->setIcon(QIcon("../res/icon/visibility-on"));
        is_password_visible = true;
    }
}

void signup_widget::on_cp_visible_OF_clicked()
{
    if (is_Cpassword_visible) {
        ui->c_password_lineEdit->setEchoMode(QLineEdit::Password);
        ui->cp_visible_OF->setIcon(QIcon("../res/icon/visibility-off"));
        is_Cpassword_visible = false;
    }
    else {
        ui->c_password_lineEdit->setEchoMode(QLineEdit::Normal);
        ui->cp_visible_OF->setIcon(QIcon("../res/icon/visibility-on"));
        is_Cpassword_visible = true;
    }
}

void signup_widget::on_renew_captcha_pushButton_clicked()
{
    captcha_gen();
}

void signup_widget::on_signup_pushButton_clicked()
{
    if (is_username_val && is_password_val && is_Cpassword_val) {
        if (ui->password_lineEdit->text() == ui->c_password_lineEdit->text()) {
            if (ui->captcha_lineEdit->text() == captcha_code) {
                if (!(is_pn_val && is_email_val)) {
                    QMessageBox::warning(this, "Forgot optinal fields", "you forgot Phone Number or Email address");
                }

                emit su(ui->username_lineEdit->text(), ui->password_lineEdit->text(), ui->pn_comboBox->currentText() + ui->pn_lineEdit->text(),
                        ui->email_lineEdit->text());
            }
            else {
                QMessageBox::critical(this, "Wrong Captcha", "Your captcha that you entered was wrong");
            }
        }
        else {
            captcha_gen();
            QMessageBox::critical(this, "Wrong Confirm Password", "Your Password and Confirm Password is not same");
        }
    }
    else {
        QMessageBox::critical(this, "Problem in fields", "Username or Password or Confirm Password is wrong");
    }
}

void signup_widget::on_goto_signin_link_linkActivated(const QString &link)
{
    emit goto_si();
}

void signup_widget::captcha_gen()
{
    QPixmap captcha_pixmap;
    QString dir_str = "../res/img/";

    captcha_code = captcha_name_arr[random(0, 9)];

    captcha_pixmap.load(dir_str.append(captcha_code));
    ui->captcha_img->setPixmap(captcha_pixmap);
}

inline int signup_widget::random(int min, int max)
{
    return (rand() % (max - min + 1)) + min;
}


void signup_widget::on_email_lineEdit_textChanged(const QString &text)
{
    int pos = 0;
    QString text_tmp = text;

    if ((email_regex_val.validate(text_tmp, pos) == QValidator::Acceptable) &&
            ((5 <= text_tmp.length()) && (text_tmp.length() <= 10))){
        is_email_val = true;
        ui->email_lineEdit->setStyleSheet("border-left-color: #8bc34a;\
                                          border-left-style: solid;\
                border-left-width: 5px;\
        padding-left:3px;");
    }
    else if (text_tmp.isEmpty()) {
        is_email_val = false;
        ui->email_lineEdit->setStyleSheet("border-left-color: #fff;\
                                          border-left-style: solid;\
                border-left-width: 5px;\
        padding-left:3px;");
    }
    else {
        is_email_val = false;
        ui->email_lineEdit->setStyleSheet("border-left-color: #f44336;\
                                          border-left-style: solid;\
                border-left-width: 5px;\
        padding-left:3px;");
    }
}

void signup_widget::on_pn_lineEdit_textChanged(const QString &text)
{
    int pos = 0;
    QString text_tmp = text;

    if ((pn_regex_val.validate(text_tmp, pos) == QValidator::Acceptable) &&
            ((5 <= text_tmp.length()) && (text_tmp.length() <= 10))){
        is_pn_val = true;
        ui->pn_lineEdit->setStyleSheet("border-left-color: #8bc34a;\
                                       border-left-style: solid;\
                border-left-width: 5px;\
        padding-left:3px;");
    }
    else if (text_tmp.isEmpty()) {
        is_pn_val = false;
        ui->pn_lineEdit->setStyleSheet("border-left-color: #fff;\
                                       border-left-style: solid;\
                border-left-width: 5px;\
        padding-left:3px;");
    }
    else {
        is_pn_val = false;
        ui->pn_lineEdit->setStyleSheet("border-left-color: #f44336;\
                                       border-left-style: solid;\
                border-left-width: 5px;\
        padding-left:3px;");
    }
}

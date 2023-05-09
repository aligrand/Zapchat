#include "signiu_page.h"
#include "ui_signiu_page.h"

signIU_page::signIU_page(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::signIU_page),
    ap_window(new auth_page()),
    userinfo_w(new UserInfoWindow()),
    database_file("./DB-temp/db-temp.json")
{
    ui->setupUi(this);

    si_w = new signin_widget(this);
    si_w->setVisible(false);
    si_w->move(293, 85);

    su_w = new signup_widget(this);
    su_w->setVisible(false);
    su_w->move(163, 30);

    si_w->setVisible(true);

    connect(ap_window, &auth_page::result_ready, this, &signIU_page::auth_result);

    connect(si_w, &signin_widget::goto_su, this, &signIU_page::goto_signup);
    connect(si_w, &signin_widget::si, this, &signIU_page::signin);
    connect(su_w, &signup_widget::goto_si, this, &signIU_page::goto_signin);
    connect(su_w, &signup_widget::su, this, &signIU_page::signup);
    connect(su_w, &signup_widget::is_username_exist, this, &signIU_page::is_username_exist);

    database_file.open(QFile::ReadWrite | QFile::Text);
    database = QJsonDocument::fromJson(database_file.readAll());
}

signIU_page::~signIU_page()
{         
    delete ui;
    delete si_w;
    delete su_w;
}

void signIU_page::goto_signin()
{
    su_w->setVisible(false);

    si_w->setVisible(true);
}

void signIU_page::goto_signup()
{
    si_w->setVisible(false);

    su_w->setVisible(true);
}

void signIU_page::signin(QString username, QString password, bool &result)
{
    bool res;

    is_username_exist(username, res);

    if(res == false)
    {
        result = false;
        return;
    }

    if(database[username]["password"] == password)
    {
        result = true;

        QFile user_file("./userinfo.txt");
        user_file.open(QFile::WriteOnly);
        QTextStream data(&user_file);
        data << username << QString(" ") << password;
        user_file.close();

        go_next_window = true;
    }
    else
    {
        result = false;
        return;
    }
}

void signIU_page::signup(QString username, QString password, QString pn, QString email)
{
    su_info[0] = username;
    su_info[1] = password;
    su_info[2] = pn;
    su_info[3] = email;

    this->hide();
    ap_window->show();
}

void signIU_page::auth_result(uByte res)
{
    // goto signup server
    if (res == 1) {
        QFile user_file("./userinfo.txt");
        user_file.open(QFile::WriteOnly);
        QTextStream data(&user_file);
        data << su_info[0] << QString(" ") << su_info[1];
        user_file.close();

        signup_server(su_info[0], su_info[1], su_info[2], su_info[3]);
        delete ap_window;
    }
    // back to signIU
    else {
        ap_window->hide();
        this->show();
    }
}

void signIU_page::is_username_exist(QString username, bool &result)
{
    if(database[username] == QJsonValue::Undefined)
    {
       result = false;
       return;
    }

    result = true;
}

void signIU_page::signup_server(QString username, QString password, QString pn, QString email)
{
    // we already check that username not exist

    QJsonObject root;
    QJsonObject info_obj;
    QJsonArray AI_arr;

    info_obj.insert("password", password);
    AI_arr.push_back(pn);
    AI_arr.push_back(email);
    info_obj.insert("AI", AI_arr);

    root = database.object();

    root.insert(username, info_obj);
    database.setObject(root);

    database_file.resize(0);

    database_file.write(database.toJson(QJsonDocument::Indented));
    database_file.close();
}

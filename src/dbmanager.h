#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>

class DBmanagerJSON
{
public:
    DBmanagerJSON(QString path);
    QString get_db_path();
    bool find_key(QString key);
    bool find_value(QString value);


protected:
    QString db_path;

private:
    QJsonDocument database;
    QFile database_file;
};

#endif // DBMANAGER_H

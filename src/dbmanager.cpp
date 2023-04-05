#include "dbmanager.h"

DBmanagerJSON::DBmanagerJSON(QString path):db_path(path), database_file(path)
{
    database_file.open(QIODevice::ReadWrite | QIODevice::Text);
    database = QJsonDocument::fromJson(database_file.readAll());
}

QString DBmanagerJSON::get_db_path()
{
    return db_path;
}

bool DBmanagerJSON::find_key(QString key)
{
    if(database[key] == QJsonValue::Undefined)
    {
       return false;
    }

    return true;
}

bool DBmanagerJSON::find_value(QString value)
{
    return true;
}

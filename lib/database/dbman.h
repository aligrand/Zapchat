// database library for SQLite

#ifndef DBMAN_H
#define DBMAN_H

#include <QSqlDatabase>
#include <QString>

class DBMan
{
public:
    DBMan(const QString path);

private:
    QSqlDatabase db;
};

#endif // DBMAN_H

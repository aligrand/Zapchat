#ifndef SQLRECORDQSTRING_H
#define SQLRECORDQSTRING_H

#include <QString>

class SqlRecordQString
{
public:
    SqlRecordQString();
    void clean();
    void end();
    SqlRecordQString &operator<<(QString element);
    operator QString();

private:
    QString recordStatment = "";
};

#endif // SQLRECORDQSTRING_H

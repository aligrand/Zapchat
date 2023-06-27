#include "sqlrecordqstring.h"

SqlRecordQString::SqlRecordQString()
{
    recordStatment = "ƒ";
}

void SqlRecordQString::clean()
{
    recordStatment = "ƒ";
}

void SqlRecordQString::end()
{
    recordStatment.remove(recordStatment.size() - 1, 1);
    recordStatment += "ƒ";
}

SqlRecordQString &SqlRecordQString::operator<<(QString element)
{
    recordStatment += element;
    recordStatment += "‡";

    return *this;
}

SqlRecordQString::operator QString()
{
    return recordStatment;
}

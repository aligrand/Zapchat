#include "sqlrecordqstring.h"

SqlRecordQString::SqlRecordQString()
{
    recordStatment = "#<$";
}

void SqlRecordQString::clean()
{
    recordStatment = "#<$";
}

void SqlRecordQString::end()
{
    recordStatment += "$>#";
}

SqlRecordQString &SqlRecordQString::operator<<(QString element)
{
    recordStatment += "&%&";
    recordStatment += element;
    recordStatment += "&%&";
}

SqlRecordQString::operator QString()
{
    return recordStatment;
}

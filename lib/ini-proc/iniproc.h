#ifndef INIPROC_H
#define INIPROC_H

#include <QString>
#include <QFile>
#include <QVector>

typedef unsigned char uByte;

class IniProc
{
public:
    IniProc(QString filename);
    ~IniProc();
    QString operator[] (QString key);
    void write(QString key, QString value);

private:
    QFile *iniFile;
    QVector<QString> settings;
};

#endif // INIPROC_H

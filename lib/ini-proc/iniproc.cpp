#include "iniproc.h"

IniProc::IniProc(QString filename)
{
    iniFile = new QFile(filename);

    iniFile->open(QIODevice::ReadWrite | QIODevice::Text);

    while (!iniFile->atEnd())
    {
        settings.push_back(iniFile->readLine().trimmed());
    }
}

IniProc::~IniProc()
{
    iniFile->close();

    delete iniFile;
}

QString IniProc::operator[](QString key)
{
    uByte i;
    for (i = 0; i < settings.length(); ++i)
    {
        if (settings[i].split("=").first() == key)
        {
            break;
        }
    }

    if (i == settings.length())
    {
        return "";
    }
    else
    {
        return settings[i].split("=").last();
    }
}

void IniProc::write(QString key, QString value)
{
    uByte i;

    for (i = 0; i < settings.length(); ++i)
    {
        if (settings[i].split("=").first() == key)
        {
            break;
        }
    }

    if (i == settings.length())
    {
        settings.push_back(key + "=" +value);
    }
    else
    {
        settings[i].split("=").last() = value;
    }

    iniFile->resize(0);

    for (i = 0; i < settings.length(); ++i)
    {
        iniFile->write((settings[i] + "\n").toStdString().c_str());
    }
}



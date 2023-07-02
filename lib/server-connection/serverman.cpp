#include "serverman.h"

ServerMan::ServerMan()
{
    socket = new QTcpSocket(this);

    connect(&readyReadTimer, &QTimer::timeout, this, &ServerMan::newMessage);
    connect(socket, &QTcpSocket::disconnected, this, &ServerMan::notConnectedProc);
    connect(this, &ServerMan::command, this, &ServerMan::commandProc);
    connect(this, &ServerMan::sendData, this, &ServerMan::sendDataProc);
    connect(this, &ServerMan::messageAsData, this, &ServerMan::messageAsDataProc);
    connect(this, &ServerMan::messageAsCommand, this, &ServerMan::messageAsCommandProc);
    connect(socket, &QTcpSocket::connected, this, &ServerMan::connectedProc);
    connect(&hostConn, &QTimer::timeout, this, &ServerMan::reConnect);
    connect(&sendTimer, &QTimer::timeout, this, &ServerMan::sendRun);

    QFile ip_port_file("server-config.conf");
    ip_port_file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString ip_port = ip_port_file.readLine();
    ip_port_file.close();

    if (ip_port.split(":").first().isEmpty())
    {
        qDebug() << "no IP";
        qApp->exit(0);
    }

    if (ip_port.split(":").last().isEmpty())
    {
        qDebug() << "no PORT";
        qApp->exit(0);
    }

    QFile _jFile("job-queue.txt");
    _jFile.open(QIODevice::ReadOnly | QIODevice::Text);
    while (!_jFile.atEnd())
    {
        job.push_front(_jFile.readLine());
    }
    _jFile.close();

    QFile _fjFile("force-job-queue.txt");
    _fjFile.open(QIODevice::ReadOnly | QIODevice::Text);
    while (!_fjFile.atEnd())
    {
        fJob.push_front(_fjFile.readLine());
    }
    _fjFile.close();

    QString cmd;
    for (int i = 0; i < job.size() ; ++i)
    {
        cmd = job[i];

        if (cmd == "UPDATE-DB" || cmd == "SET-PASS" || cmd == "UN-EXIST" ||
                cmd == "ROOM-EXIST" || cmd == "LOGIN" || cmd == "MESSAGE-INDEX" ||
                cmd == "REMOVE-USER" || cmd == "REMOVE-ROOM" || cmd == "ARRIVE")
        {
            fJob.push_back(cmd);
            job.removeAt(i);
        }
    }

    hostConn.setInterval(5000);
    sendTimer.setInterval(50);

    hostConn.start();

    readyReadTimer.start(50);

    socket->connectToHost(ip_port.split(":").first(), ip_port.split(":").last().toUShort());
}

ServerMan::~ServerMan()
{
    socket->deleteLater();

    QFile _jFile("job-queue.txt");
    _jFile.open(QIODevice::WriteOnly | QIODevice::Text);
    _jFile.resize(0);
    while (j_delPending.size() > 0)
    {
        _jFile.write((j_delPending.last() + "\n").toStdString().c_str());
        j_delPending.pop_back();
    }
    while (job.size() > 0)
    {
        _jFile.write((job.last() + "\n").toStdString().c_str());
        job.pop_back();
    }
    _jFile.close();

    QFile _fjFile("force-job-queue.txt");
    _fjFile.open(QIODevice::WriteOnly | QIODevice::Text);
    _fjFile.resize(0);
    while (fJob.size() > 0)
    {
        _fjFile.write((fJob.last() + "\n").toStdString().c_str());
        fJob.pop_back();
    }
    _fjFile.close();
}

void ServerMan::sendRun()
{
    QString final_cmd;
    QByteArray header;
    QByteArray data;

    if (fJob.size() > 0)
    {
        final_cmd = fJob.last();
        fJob.pop_back();
    }
    else if (job.size() > 0)
    {
        final_cmd = job.last();
        job.pop_back();
    }
    else
    {
        return;
    }

    qDebug() << "<SEND>" << final_cmd;

    if (final_cmd.split(" ").first() == "_UPLOAD_")
    {
        if (final_cmd.split(" ").last().isEmpty())
        {
            return;
        }

        QString filePath = QString::fromStdString(final_cmd.toStdString().substr(static_cast<size_t>(final_cmd.indexOf(" ")) + 1));
        QFile _file(filePath);
        if (!_file.open(QIODevice::ReadOnly))
        {
            qDebug() << "File not found -> " << filePath;

            return;
        }

        j_delPending.push_front(final_cmd);

        data = _file.readAll();
        _file.close();

        header.append(char(1));
        header.append(filePath.split("/").last().split(".").first().toUtf8());
        header.resize(61);
        header.append(filePath.split("/").last().split(".").last().toUtf8());
        header.resize(71);
    }
    else
    {
        j_delPending.push_front(final_cmd);

        data = final_cmd.toUtf8();

        header.append(char(0));
        header.append("");
        header.resize(61);
        header.append("");
        header.resize(71);
    }

    header.append(data);

    emit sendData(header);
}

NetworkState ServerMan::getNetworkState()
{
    return ns;
}

bool ServerMan::getIsLogin()
{
    return isLogin;
}

void ServerMan::notConnectedProc()
{
    sendTimer.stop();

    ns = NetworkState::Offline;

    QMessageBox::warning(nullptr, "Zapchat", "Connection to server disconnected");

    emit notConnected();

    hostConn.start();
}

void ServerMan::connectedProc()
{
    sendTimer.start();

    ns = NetworkState::Online;

    emit connected();

    hostConn.stop();
}

void ServerMan::reConnect()
{
    QFile ip_port_file("server-config.conf");
    ip_port_file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString ip_port = ip_port_file.readLine();
    ip_port_file.close();

    socket->connectToHost(ip_port.split(":").first(), ip_port.split(":").last().toUShort());
}

void ServerMan::sendDataProc(QByteArray sData)
{
    QDataStream socketStream(socket);

    socketStream << sData;
}

void ServerMan::commandProc(QString cmd)
{
    QString cmdName = cmd.split(" ").first();

    if (cmdName == "UPDATE-DB" || cmdName == "SET-PASS" || cmdName == "UN-EXIST" ||
            cmdName == "ROOM-EXIST" || cmdName == "LOGIN" || cmdName == "MESSAGE-INDEX" ||
            cmdName == "REMOVE-USER" || cmdName == "REMOVE-ROOM" || cmdName == "ARRIVE")
    {
        fJob.push_front(cmd);
    }
    else
    {
        job.push_front(cmd);
    }
}

void ServerMan::newMessage()
{
    QByteArray data;

    if (socket->bytesAvailable() == 0)
    {
        return;
    }

    QDataStream socketStream(socket);
    socketStream.setVersion(QDataStream::Qt_5_13);

    socketStream.startTransaction();
    socketStream >> data;

    if (!socketStream.commitTransaction())
    {
        return;
    }

    if (data.at(0) == 0)
    {
        emit messageAsCommand(data);
    }
    else
    {
        emit messageAsData(data);
    }
}

void ServerMan::messageAsDataProc(QByteArray rData)
{
    QString fileName, fileFormat, dir;

    fileName = rData.mid(1, 60);
    fileFormat = rData.mid(61, 10);

    qDebug() << "<RECIVE>" << fileName << "." << fileFormat;

    if (fileName[fileName.size() - 1] == "I")
    {
        dir = "Images/";
    }
    else if (fileName[fileName.size() - 1] == "V")
    {
        dir = "Videos/";
    }
    else if (fileName[fileName.size() - 1] == "A")
    {
        dir = "Audios/";
    }
    else if (fileName[fileName.size() - 1] == "F")
    {
        dir = "Files/";
    }
    else
    {
        dir = "Profiles/";
    }

    emit command("ARRIVE");

    QFile::remove(dir + fileName + "." + fileFormat);
    QFile file(dir + fileName + "." + fileFormat);
    file.open(QIODevice::WriteOnly);
    file.write(rData.mid(71));
    file.close();

    emit dirUpdated();
}

void ServerMan::messageAsCommandProc(QByteArray rData)
{
    QString cmd = rData.mid(71);
    QString cmdName = cmd.split(" ").first();
    QString cmdArgs = QString::fromStdString(cmd.toStdString().substr(static_cast<size_t>(cmd.indexOf(" ")) + 1));

    qDebug() << "<RECIVE>" << cmd;

    QRegularExpression dbRegex("(?<=ƒ)[^ƒ]*(?=ƒ)");
    QRegularExpressionMatch dbMatch;
    QStringList dbList;
    QSqlQuery sqlQuery;

    if (cmdName == "ADD-ROOM")
    {
        dbMatch = dbRegex.match(cmdArgs);
        dbList = dbMatch.captured(0).split("‡");

        sqlQuery.prepare("INSERT INTO rooms (id, name, photoADDRESS, info, type, pin) "
                         "VALUES (?, ?, ?, ?, ?, ?)");
        sqlQuery.addBindValue(dbList[0]);
        sqlQuery.addBindValue(dbList[1]);
        sqlQuery.addBindValue(dbList[2]);
        sqlQuery.addBindValue(dbList[3]);
        sqlQuery.addBindValue(dbList[4]);
        sqlQuery.addBindValue(dbList[5]);
        sqlQuery.exec();

        sqlQuery.prepare("INSERT INTO new_messages (roomID) VALUES (?)");
        sqlQuery.addBindValue(dbList[0]);
        sqlQuery.exec();

        emit databaseUpdated("rooms");
    }
    else if (cmdName == "ADD-USER")
    {
        dbMatch = dbRegex.match(cmdArgs);
        dbList = dbMatch.captured(0).split("‡");

        sqlQuery.prepare("SELECT COUNT(*) FROM users WHERE username=?");
        sqlQuery.addBindValue(dbList[0]);
        sqlQuery.exec();
        sqlQuery.first();
        int sqlsize = sqlQuery.value("COUNT(*)").toInt();

        if (sqlsize != 0)
        {
            sqlQuery.prepare("UPDATE users SET emailAddress=?, phoneNumber=?, name=?, "
                             "photoADDRESS=?, info=?, isOnline=? WHERE username=?");
            sqlQuery.addBindValue(dbList[1]);
            sqlQuery.addBindValue(dbList[2]);
            sqlQuery.addBindValue(dbList[3]);
            sqlQuery.addBindValue(dbList[4]);
            sqlQuery.addBindValue(dbList[5]);
            sqlQuery.addBindValue(dbList[6]);
            sqlQuery.addBindValue(dbList[0]);
            sqlQuery.exec();
        }
        else
        {
            sqlQuery.prepare("INSERT INTO users (username, emailAddress, phoneNumber, name, photoADDRESS,"
                             " info, isOnline) VALUES (?, ?, ?, ?, ?, ?, ?)");
            sqlQuery.addBindValue(dbList[0]);
            sqlQuery.addBindValue(dbList[1]);
            sqlQuery.addBindValue(dbList[2]);
            sqlQuery.addBindValue(dbList[3]);
            sqlQuery.addBindValue(dbList[4]);
            sqlQuery.addBindValue(dbList[5]);
            sqlQuery.addBindValue(dbList[6]);
            sqlQuery.exec();
        }

        emit databaseUpdated("users");
    }
    else if (cmdName == "ADD-PARTICIPANT")
    {
        dbMatch = dbRegex.match(cmdArgs);
        dbList = dbMatch.captured(0).split("‡");

        sqlQuery.prepare("INSERT INTO participants (userID, roomID, role) VALUES (?, ?, ?)");
        sqlQuery.addBindValue(dbList[0]);
        sqlQuery.addBindValue(dbList[1]);
        sqlQuery.addBindValue(dbList[2]);
        sqlQuery.exec();

        emit databaseUpdated("participants");
    }
    else if (cmdName == "ADD-MESSAGE")
    {
        dbMatch = dbRegex.match(cmdArgs);
        dbList = dbMatch.captured(0).split("‡");

        sqlQuery.prepare("INSERT INTO messages (id, roomID, userID, key, DT, replyID, text, "
                         "imageADDRESS, videoADDRESS, audioADDRESS, fileADDRESS) "
                         "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
        sqlQuery.addBindValue(dbList[0]);
        sqlQuery.addBindValue(dbList[1]);
        sqlQuery.addBindValue(dbList[2]);
        sqlQuery.addBindValue(dbList[3]);
        sqlQuery.addBindValue(dbList[4]);
        sqlQuery.addBindValue(dbList[5]);
        sqlQuery.addBindValue(dbList[6]);
        sqlQuery.addBindValue(dbList[7]);
        sqlQuery.addBindValue(dbList[8]);
        sqlQuery.addBindValue(dbList[9]);
        sqlQuery.addBindValue(dbList[10]);
        sqlQuery.exec();

        sqlQuery.prepare("UPDATE new_messages SET count=count+1 WHERE roomID=?");
        sqlQuery.addBindValue(dbList[1]);
        sqlQuery.exec();

        emit databaseUpdated("messages-" + dbList[1]);
    }
    else if (cmdName == "REMOVE-MESSAGE")
    {
        sqlQuery.prepare("SELECT roomID FROM messages WHERE id=?");
        sqlQuery.addBindValue(cmdArgs);
        sqlQuery.exec();
        sqlQuery.first();
        QString _roomID = sqlQuery.value("roomID").toString();

        sqlQuery.prepare("DELETE FROM messages WHERE id=?");
        sqlQuery.addBindValue(cmdArgs);
        sqlQuery.exec();

        sqlQuery.prepare("UPDATE new_messages SET count=count+1 WHERE roomID=?");
        sqlQuery.addBindValue(_roomID);
        sqlQuery.exec();

        emit databaseUpdated("messages-" + _roomID + "-remove");
    }
    else if (cmdName == "REMOVE-USER")
    {
        sqlQuery.prepare("DELETE FROM users WHERE username=?");
        sqlQuery.addBindValue(cmdArgs);
        sqlQuery.exec();

        emit databaseUpdated("users");
    }
    else if (cmdName == "REMOVE-PARTICIPANT")
    {
        sqlQuery.prepare("DELETE FROM participants WHERE roomID=? AND userID=?");
        sqlQuery.addBindValue(cmdArgs.split(" ").first());
        sqlQuery.addBindValue(cmdArgs.split(" ").last());
        sqlQuery.exec();

        emit databaseUpdated("participants");
    }
    else if (cmdName == "REMOVE-ROOM")
    {
        sqlQuery.prepare("DELETE FROM rooms WHERE id=?");
        sqlQuery.addBindValue(cmdArgs);
        sqlQuery.exec();

        emit databaseUpdated("rooms");
    }
    else if (cmdName == "EDIT-ROOM")
    {
        dbMatch = dbRegex.match(cmdArgs);
        dbList = dbMatch.captured(0).split("‡");

        sqlQuery.prepare("UPDATE rooms SET name=?, photoADDRESS=?, info=?, pin=? WHERE id=?");
        sqlQuery.addBindValue(dbList[1]);
        sqlQuery.addBindValue(dbList[2]);
        sqlQuery.addBindValue(dbList[3]);
        sqlQuery.addBindValue(dbList[5]);
        sqlQuery.addBindValue(dbList[0]);
        sqlQuery.exec();

        emit databaseUpdated("rooms-edit");
    }
    else if (cmdName == "EDIT-USER")
    {
        dbMatch = dbRegex.match(cmdArgs);
        dbList = dbMatch.captured(0).split("‡");

        sqlQuery.prepare("UPDATE users SET emailAddress=?, phoneNumber=?, name=?, "
                         "photoADDRESS=?, info=?, isOnline=? WHERE id=?");
        sqlQuery.addBindValue(dbList[1]);
        sqlQuery.addBindValue(dbList[2]);
        sqlQuery.addBindValue(dbList[3]);
        sqlQuery.addBindValue(dbList[4]);
        sqlQuery.addBindValue(dbList[5]);
        sqlQuery.addBindValue(dbList[6]);
        sqlQuery.addBindValue(dbList[0]);
        sqlQuery.exec();

        emit databaseUpdated("users");
    }
    else if (cmdName == "LOGIN-RESULT")
    {
        (cmdArgs == 1) ? (isLogin = true) : (isLogin = false);

        emit loginResult(cmdArgs.toInt());
    }
    else if (cmdName == "UN-EXIST-RESULT")
    {
        bool res = cmdArgs.split(" ").last() == "1" ? true : false;

        emit userNameExistResult(res, cmdArgs.split(" ").first());
    }
    else if (cmdName == "ROOM-EXIST-RESULT")
    {
        bool res = cmdArgs.split(" ").last() == "1" ? true : false;

        emit idExistResult(res, cmdArgs.split(" ").first());
    }
    else if (cmdName == "MESSAGE-INDEX-RESULT")
    {
        QFile _file("message-index.txt");
        _file.open(QIODevice::WriteOnly | QIODevice::Text);
        _file.resize(0);
        _file.write(cmdArgs.toStdString().c_str());
        _file.close();
    }
    else if (cmdName == "ARRIVE")
    {
        j_delPending.pop_back();
    }

    if (cmdName != "ARRIVE")
    {
        emit command("ARRIVE");
    }
}


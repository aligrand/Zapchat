#include "messagewidget.h"
#include "ui_messagewidget.h"

MessageWidget::MessageWidget(QString messageID, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MessageWidget)
{
    ui->setupUi(this);

    connect(server, &ServerMan::dirUpdated, this, &MessageWidget::checkResUpdated);

    ui->text->setTextInteractionFlags(Qt::TextSelectableByMouse);

    mID = messageID;

    QSqlQuery sqlQuery;

    sqlQuery.prepare("SELECT * FROM messages WHERE id=?");
    sqlQuery.addBindValue(messageID);
    sqlQuery.exec();
    sqlQuery.first();

    rID = sqlQuery.value("roomID").toString();

    if (sqlQuery.value("replyID").toString().isEmpty())
    {
        delete ui->reply;
    }
    else
    {
        ui->replyLable->setText(sqlQuery.value("replyID").toString());
    }

    if (sqlQuery.value("text").toString().isEmpty())
    {
        delete ui->text;
    }
    else
    {
        ui->text->setText(sqlQuery.value("text").toString());

        int number_of_lines = ui->text->document()->blockCount();

        QFontMetrics font_metrics(ui->text->font());
        int font_height = font_metrics.height();

        int height = font_height * number_of_lines;

        ui->text->setMinimumHeight(height + 20);
        ui->text->setMaximumHeight(height + 20);
    }

    if (sqlQuery.value("imageADDRESS").toString().isEmpty())
    {
        delete ui->imageLable;
    }
    else
    {
        if (QFile::exists("Images/" + sqlQuery.value("imageADDRESS").toString()))
        {
            ui->imageLable->setPixmap(QPixmap("Images/" + sqlQuery.value("imageADDRESS").toString()));

            iPathExist = true;
        }

        iPath = sqlQuery.value("imageADDRESS").toString();

        ui->imageLable->setContextMenuPolicy(Qt::CustomContextMenu);
    }

    if (sqlQuery.value("videoADDRESS").toString().isEmpty())
    {
        delete ui->video;
    }
    else
    {
        if (QFile::exists("Videos/" + sqlQuery.value("videoADDRESS").toString()))
        {
            videoPlayer = new QMediaPlayer();
            videoPlayer->setMedia(QUrl::fromLocalFile("Videos/" + sqlQuery.value("videoADDRESS").toString()));
            videoPlayer->setVideoOutput(ui->video);
            videoPlayer->setVolume(100);

            vPathExist = true;
        }
        else
        {
            videoPlayer = new QMediaPlayer();
            videoPlayer->setMedia(QUrl::fromLocalFile("../res/vid/novideo.gif"));
            videoPlayer->setVideoOutput(ui->video);
            videoPlayer->setVolume(100);
        }

        vPath = sqlQuery.value("videoADDRESS").toString();

        ui->video->setContextMenuPolicy(Qt::CustomContextMenu);
    }

    if (sqlQuery.value("audioADDRESS").toString().isEmpty())
    {
        delete ui->audio;
    }
    else
    {
        if (QFile::exists("Audios/" + sqlQuery.value("audioADDRESS").toString()))
        {
            audioPlayer = new QMediaPlayer();
            audioPlayer->setMedia(QUrl::fromLocalFile("Audios/" + sqlQuery.value("audioADDRESS").toString()));
            audioPlayer->setVolume(100);
            ui->audioPlayPushButton->setIcon(QIcon("../res/icon/play_icon.png"));

            aPathExist = true;

            connect(audioPlayer, &QMediaPlayer::positionChanged, this, &MessageWidget::audioBarChange);

            audioPlayer->setNotifyInterval(10);
        }

        aPath = sqlQuery.value("audioADDRESS").toString();

        ui->audio->setContextMenuPolicy(Qt::CustomContextMenu);
    }

    if (sqlQuery.value("fileADDRESS").toString().isEmpty())
    {
        delete ui->file;
    }
    else
    {
        ui->fileLabel->setText(sqlQuery.value("fileADDRESS").toString());

        if (QFile::exists("Files/" + sqlQuery.value("fileADDRESS").toString()))
        {
            ui->fileDownloadPushButton->setIcon(QIcon("../res/icon/folder_open_icon.png"));

            fPathExist = true;
        }

        fPath = sqlQuery.value("fileADDRESS").toString();
    }

    ui->dtLable->setText(sqlQuery.value("DT").toString());

    userID = sqlQuery.value("userID").toString();

    sqlQuery.prepare("SELECT photoADDRESS, name FROM users WHERE username=?");
    sqlQuery.addBindValue(userID);
    sqlQuery.exec();
    sqlQuery.first();

    ui->profilePicLable->setIcon(QIcon(QString("Profiles/") + sqlQuery.value("photoADDRESS").toString()));
    ui->senderNameLable->setText(sqlQuery.value("name").toString());

    this->setContextMenuPolicy(Qt::CustomContextMenu);
}

MessageWidget::~MessageWidget()
{
    delete ui;

    if (audioPlayer != nullptr)
    {
        audioPlayer->deleteLater();
    }
    if (videoPlayer != nullptr)
    {
        videoPlayer->deleteLater();
    }
    if (senderProfile != nullptr)
    {
        senderProfile->deleteLater();
    }
    if (contextMenu != nullptr)
    {
        contextMenu->deleteLater();
    }
    if (replyMes != nullptr)
    {
        replyMes->deleteLater();
    }
}

void MessageWidget::on_replyButton_clicked()
{
    replyMes = new MessageViewerWindow("id='" + ui->replyLable->text() + "'");

    replyMes->show();
}

void MessageWidget::on_horizontalSlider_sliderMoved(int position)
{
    audioPlayer->pause();
    qint64 seekTime = (position * audioPlayer->duration()) / 100;
    audioPlayer->setPosition(seekTime);
    audioPlayer->play();
}

void MessageWidget::on_profilePicLable_clicked()
{
    senderProfile = new RoomInfoPanel(true, userID);
    senderProfile->setWindowIcon(QIcon("../res/icon/Zapchat-icon.png"));
    senderProfile->setWindowTitle("Zapchat");
    senderProfile->show();
}

void MessageWidget::on_audioPlayPushButton_clicked()
{
    if (!aPathExist)
    {
        emit server->command("DOWNLOAD " + aPath);

        return;
    }

    if (audio_played)
    {
        audioPlayer->pause();
        ui->audioPlayPushButton->setIcon(QIcon("../res/icon/play_icon.png"));
        audio_played = false;
    }
    else
    {
        audioPlayer->play();
        ui->audioPlayPushButton->setIcon(QIcon("../res/icon/pause_icon.png"));
        audio_played = true;
    }
}

void MessageWidget::on_fileDownloadPushButton_clicked()
{
    if (!fPathExist)
    {
        emit server->command("DOWNLOAD " + fPath);

        return;
    }

    QDesktopServices::openUrl(QUrl::fromLocalFile("Files/" + fPath));
}

void MessageWidget::on_imageLable_customContextMenuRequested(const QPoint &pos)
{
    delete contextMenu;
    contextMenu = new QMenu();

    contextMenu->addAction("Open");

    if (iPathExist)
    {
        contextMenu->addAction("Save");
    }
    else
    {
        contextMenu->addAction("Download");
    }

    connect(contextMenu, &QMenu::triggered, this, &MessageWidget::contextMenuProc);

    contextMenuParent = 'I';

    contextMenu->popup(ui->imageLable->mapToGlobal(pos));
}

void MessageWidget::on_video_customContextMenuRequested(const QPoint &pos)
{
    delete contextMenu;
    contextMenu = new QMenu();

    contextMenu->addAction("Open");

    if (vPathExist)
    {
        contextMenu->addAction("Save");
    }
    else
    {
        contextMenu->addAction("Download");
    }

    if (video_played)
    {
        contextMenu->addAction("Pause");
    }
    else
    {
        contextMenu->addAction("Play");
    }

    connect(contextMenu, &QMenu::triggered, this, &MessageWidget::contextMenuProc);

    contextMenuParent = 'V';

    contextMenu->popup(ui->video->mapToGlobal(pos));
}

void MessageWidget::on_audio_customContextMenuRequested(const QPoint &pos)
{
    delete contextMenu;
    contextMenu = new QMenu();

    contextMenu->addAction("Open");

    if (aPathExist)
    {
        contextMenu->addAction("Save");
    }

    connect(contextMenu, &QMenu::triggered, this, &MessageWidget::contextMenuProc);

    contextMenuParent = 'A';

    contextMenu->popup(ui->audio->mapToGlobal(pos));
}

void MessageWidget::on_MessageWidget_customContextMenuRequested(const QPoint &pos)
{
    delete contextMenu;
    contextMenu = new QMenu();

    QSqlQuery sqlQuery;
    sqlQuery.prepare("SELECT role FROM participants WHERE roomID=? AND userID=?");
    sqlQuery.addBindValue(rID);
    sqlQuery.addBindValue(myUsername);
    sqlQuery.exec();
    sqlQuery.first();

    contextMenu->addAction("ID");

    if (sqlQuery.value("role").toString() == "M")
    {
        contextMenu->addAction("Delete");
        contextMenu->addAction("Pin it");
    }
    else
    {
        if (userID == myUsername)
        {
            contextMenu->addAction("Delete");
        }
    }

    connect(contextMenu, &QMenu::triggered, this, &MessageWidget::contextMenuProc);

    contextMenu->popup(this->mapToGlobal(pos));
}

void MessageWidget::on_file_customContextMenuRequested(const QPoint &pos)
{
    delete contextMenu;
    contextMenu = new QMenu();

    if (fPathExist)
    {
        contextMenu->addAction("Save");
    }

    connect(contextMenu, &QMenu::triggered, this, &MessageWidget::contextMenuProc);

    contextMenuParent = 'F';

    contextMenu->popup(pos);
}

void MessageWidget::contextMenuProc(QAction *action)
{
    if (action->text() == "Open")
    {
        if (contextMenuParent == 'I')
        {
            QDesktopServices::openUrl(QUrl::fromLocalFile("Images/" + iPath));
        }
        else if (contextMenuParent == 'V')
        {
            QDesktopServices::openUrl(QUrl::fromLocalFile("Videos/" + vPath));
        }
        else // audio
        {
            QDesktopServices::openUrl(QUrl::fromLocalFile("Audios/" + aPath));
        }
    }
    else if (action->text() == "Save")
    {
        if (contextMenuParent == 'I')
        {
            QString outputFolder = QFileDialog::getExistingDirectory(nullptr, ("Select Output Folder"), QDir::currentPath());

            if (QFile::exists(outputFolder + "/" + iPath))
            {
                QFile::remove(outputFolder + "/" + iPath);
            }
            QFile::copy(iPath, outputFolder + "/" + iPath);
        }
        else if (contextMenuParent == 'V')
        {
            QString outputFolder = QFileDialog::getExistingDirectory(nullptr, ("Select Output Folder"), QDir::currentPath());

            if (QFile::exists(outputFolder + "/" + vPath))
            {
                QFile::remove(outputFolder + "/" + vPath);
            }
            QFile::copy(vPath, outputFolder + "/" + vPath);
        }
        else if (contextMenuParent == 'A')
        {
            QString outputFolder = QFileDialog::getExistingDirectory(nullptr, ("Select Output Folder"), QDir::currentPath());

            if (QFile::exists(outputFolder + "/" + aPath))
            {
                QFile::remove(outputFolder + "/" + aPath);
            }
            QFile::copy(aPath, outputFolder + "/" + aPath);
        }
        else
        {
            QString outputFolder = QFileDialog::getExistingDirectory(nullptr, ("Select Output Folder"), QDir::currentPath());

            if (QFile::exists(outputFolder + "/" + fPath))
            {
                QFile::remove(outputFolder + "/" + fPath);
            }
            QFile::copy(fPath, outputFolder + "/" + fPath);
        }
    }
    else if (action->text() == "Download")
    {
        if (contextMenuParent == 'I')
        {
            emit server->command("DOWNLOAD " + iPath);
        }
        else
        {
            emit server->command("DOWNLOAD " + vPath);
        }
    }
    else if (action->text() == "Pin it")
    {
        QSqlQuery sqlQuery;

        sqlQuery.prepare("UPDATE rooms SET pin=? WHERE id=?");
        sqlQuery.addBindValue(mID);
        sqlQuery.addBindValue(rID);
        sqlQuery.exec();
    }
    else if (action->text() == "ID")
    {
        QClipboard *clipboard = QGuiApplication::clipboard();
        clipboard->setText(mID);
        QMessageBox::information(nullptr, "Zapchat", "MessageID copied into clipboard");
    }
    else if (action->text() == "Pause")
    {
        videoPlayer->pause();
        video_played = false;
    }
    else if (action->text() == "Play")
    {
        videoPlayer->play();
        video_played = true;
    }
    else
    {
        QSqlQuery sqlQuery;
        sqlQuery.prepare("DELETE FROM messages WHERE id=?");
        sqlQuery.addBindValue(mID);
        sqlQuery.exec();

        emit server->command("REMOVE-MESSAGE " + mID);
    }
}

void MessageWidget::audioBarChange(qint64 pos)
{
    qint64 tmpPos = (pos * ui->horizontalSlider->maximum()) / audioPlayer->duration();
    ui->horizontalSlider->setValue(tmpPos);
}

void MessageWidget::checkResUpdated()
{
    if (QFile::exists("Images/" + iPath) && !iPathExist)
    {
        ui->imageLable->setPixmap(QPixmap("Images/" + iPath));
    }
    if (QFile::exists("Videos/" + vPath) && !vPathExist)
    {
        videoPlayer = new QMediaPlayer();
        videoPlayer->setMedia(QUrl::fromLocalFile("Videos/" + vPath));
        videoPlayer->setVideoOutput(ui->video);
        videoPlayer->setVolume(100);

        vPathExist = true;
    }

    if (QFile::exists("Audios/" + aPath) && !aPathExist)
    {
        audioPlayer = new QMediaPlayer();
        audioPlayer->setMedia(QUrl::fromLocalFile("Audios/" + aPath));
        audioPlayer->setVolume(100);
        ui->audioPlayPushButton->setIcon(QIcon("../res/icon/play_icon.png"));

        aPathExist = true;
    }

    if (QFile::exists("Files/" + fPath) && !fPathExist)
    {
        ui->fileDownloadPushButton->setIcon(QIcon("../res/icon/folder_open_icon.png"));

        fPathExist = true;
    }
}



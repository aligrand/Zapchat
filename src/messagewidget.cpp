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

    rID = sqlQuery.value("roomID").toString();

    if (sqlQuery.value("replyID").isNull())
    {
        delete ui->reply;
    }
    else
    {
        ui->replyLable->setText(sqlQuery.value("replyID").toString());
    }

    if (sqlQuery.value("text").isNull())
    {
        delete ui->text;
    }
    else
    {
        ui->text->setText(sqlQuery.value("text").toString());
    }

    if (sqlQuery.value("imageADDRESS").isNull())
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

        iPath = "Images/" + sqlQuery.value("imageADDRESS").toString();
    }

    if (sqlQuery.value("videoADDRESS").isNull())
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
            videoPlayer->play();

            vPathExist = true;
        }

        vPath = "Videos/" + sqlQuery.value("videoADDRESS").toString();
    }

    if (sqlQuery.value("audioADDRESS").isNull())
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
        }

        aPath = "Audios/" + sqlQuery.value("audioADDRESS").toString();
    }

    if (sqlQuery.value("fileADDRESS").isNull())
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

        fPath = "Files/" + sqlQuery.value("fileADDRESS").toString();
    }

    ui->dtLable->setText(sqlQuery.value("DT").toString());

    userID = sqlQuery.value("userID").toString();

    sqlQuery.prepare("SELECT photoADDRESS, name FROM users WHERE username=?");
    sqlQuery.addBindValue(userID);
    sqlQuery.exec();

    ui->profilePicLable->setPixmap(QPixmap(QString("Cache/") + sqlQuery.value("photoADDRESS").toString()));
    ui->senderNameLable->setText(sqlQuery.value("name").toString());
}

MessageWidget::~MessageWidget()
{
    delete ui;
}

void MessageWidget::on_replyButton_clicked()
{
    replyMes = new MessageViewerWindow("id=" + mID);

    replyMes->show();
}

void MessageWidget::on_horizontalSlider_sliderMoved(int position)
{
    qint64 seekTime = (position / 100) * audioPlayer->duration();
    audioPlayer->setPosition(seekTime);
}

void MessageWidget::on_profilePicLable_linkActivated(const QString &link)
{
    senderProfile = new RoomInfoPanel(true, userID);
    senderProfile->show();
}

void MessageWidget::on_audioPlayPushButton_clicked()
{
    if (!aPathExist)
    {
        emit server->command("DOWNLOAD " + aPath.split("/").last());

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
        emit server->command("DOWNLOAD " + fPath.split("/").last());

        return;
    }

    QDesktopServices::openUrl(QUrl(fPath));
}

void MessageWidget::on_imageLable_linkActivated(const QString &link)
{
    if (!iPathExist)
    {
        emit server->command("DOWNLOAD " + iPath.split("/").last());

        return;
    }
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

    connect(contextMenu, &QMenu::triggered, this, &MessageWidget::contextMenuProc);

    contextMenuParent = 'I';

    contextMenu->popup(pos);
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

    connect(contextMenu, &QMenu::triggered, this, &MessageWidget::contextMenuProc);

    contextMenuParent = 'V';

    contextMenu->popup(pos);
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

    contextMenu->popup(pos);
}

void MessageWidget::on_MessageWidget_customContextMenuRequested(const QPoint &pos)
{
    delete contextMenu;
    contextMenu = new QMenu();

    contextMenu->addAction("Delete");
    contextMenu->addAction("Pin it");

    connect(contextMenu, &QMenu::triggered, this, &MessageWidget::contextMenuProc);

    contextMenu->popup(pos);
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
            QDesktopServices::openUrl(QUrl(iPath));
        }
        else if (contextMenuParent == 'V')
        {
            QDesktopServices::openUrl(QUrl(vPath));
        }
        else // audio
        {
            QDesktopServices::openUrl(QUrl(aPath));
        }
    }
    else if (action->text() == "Save")
    {
        if (contextMenuParent == 'I')
        {
            QString outputFolder = QFileDialog::getExistingDirectory(nullptr, ("Select Output Folder"), QDir::currentPath());

            QFile::copy(iPath, outputFolder + iPath.split("/").last());
        }
        else if (contextMenuParent == 'V')
        {
            QString outputFolder = QFileDialog::getExistingDirectory(nullptr, ("Select Output Folder"), QDir::currentPath());

            QFile::copy(vPath, outputFolder + vPath.split("/").last());
        }
        else if (contextMenuParent == 'A')
        {
            QString outputFolder = QFileDialog::getExistingDirectory(nullptr, ("Select Output Folder"), QDir::currentPath());

            QFile::copy(aPath, outputFolder + aPath.split("/").last());
        }
        else
        {
            QString outputFolder = QFileDialog::getExistingDirectory(nullptr, ("Select Output Folder"), QDir::currentPath());

            QFile::copy(fPath, outputFolder + fPath.split("/").last());
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
    else
    {
        emit server->command("REMOVE-MESSAGE " + mID);
    }
}

void MessageWidget::checkResUpdated()
{
    if (QFile::exists(iPath) && !iPathExist)
    {
        ui->imageLable->setPixmap(QPixmap(iPath));
    }
    if (QFile::exists(vPath) && !vPathExist)
    {
        videoPlayer = new QMediaPlayer();
        videoPlayer->setMedia(QUrl::fromLocalFile(vPath));
        videoPlayer->setVideoOutput(ui->video);
        videoPlayer->setVolume(100);
        videoPlayer->play();

        vPathExist = true;
    }

    if (QFile::exists(aPath) && !aPathExist)
    {
        audioPlayer = new QMediaPlayer();
        audioPlayer->setMedia(QUrl::fromLocalFile(vPath));
        audioPlayer->setVolume(100);
        ui->audioPlayPushButton->setIcon(QIcon("../res/icon/play_icon.png"));

        aPathExist = true;
    }

    if (QFile::exists(fPath) && !fPathExist)
    {
        ui->fileDownloadPushButton->setIcon(QIcon("../res/icon/folder_open_icon.png"));

        fPathExist = true;
    }
}



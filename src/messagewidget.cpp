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
            videoPlayer->play();

            vPathExist = true;
        }

        vPath = sqlQuery.value("videoADDRESS").toString();
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
        }

        aPath = sqlQuery.value("audioADDRESS").toString();
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

    QDesktopServices::openUrl(QUrl("Files/" + fPath));
}

void MessageWidget::on_imageLable_linkActivated(const QString &link)
{
    if (!iPathExist)
    {
        emit server->command("DOWNLOAD " + iPath);

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
            QDesktopServices::openUrl(QUrl("Images/" + iPath));
        }
        else if (contextMenuParent == 'V')
        {
            QDesktopServices::openUrl(QUrl("Videos/" + vPath));
        }
        else // audio
        {
            QDesktopServices::openUrl(QUrl("Audios/" + aPath));
        }
    }
    else if (action->text() == "Save")
    {
        if (contextMenuParent == 'I')
        {
            QString outputFolder = QFileDialog::getExistingDirectory(nullptr, ("Select Output Folder"), QDir::currentPath());

            QFile::copy(iPath, outputFolder + "/" + iPath);
        }
        else if (contextMenuParent == 'V')
        {
            QString outputFolder = QFileDialog::getExistingDirectory(nullptr, ("Select Output Folder"), QDir::currentPath());

            QFile::copy(vPath, outputFolder + "/" + vPath);
        }
        else if (contextMenuParent == 'A')
        {
            QString outputFolder = QFileDialog::getExistingDirectory(nullptr, ("Select Output Folder"), QDir::currentPath());

            QFile::copy(aPath, outputFolder + "/" + aPath);
        }
        else
        {
            QString outputFolder = QFileDialog::getExistingDirectory(nullptr, ("Select Output Folder"), QDir::currentPath());

            QFile::copy(fPath, outputFolder + "/" + fPath);
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
        videoPlayer->play();

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



#ifndef MESSAGEWIDGET_H
#define MESSAGEWIDGET_H

#include <QWidget>
#include <QMediaPlayer>
#include <QSqlQuery>
#include <QDesktopServices>
#include <QUrl>
#include <QPixmap>
#include <QIcon>
#include <QFile>
#include <QMenu>
#include <QFileDialog>
#include <QClipboard>
#include <QString>
#include <QStringList>

#include "lib/server-connection/serverman.h"
#include "lib/server-connection/sqlrecordqstring.h"
#include "roominfopanel.h"
#include "messageviewerwindow.h"

extern ServerMan *server;

class MessageViewerWindow;

namespace Ui {
class MessageWidget;
}

class MessageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MessageWidget(QString messageID, QWidget *parent = nullptr);
    ~MessageWidget();

private slots:
    void on_replyButton_clicked();

    void on_horizontalSlider_sliderMoved(int position);

    void on_profilePicLable_clicked();

    void on_audioPlayPushButton_clicked();

    void on_fileDownloadPushButton_clicked();

    void on_imageLable_customContextMenuRequested(const QPoint &pos);

    void on_video_customContextMenuRequested(const QPoint &pos);

    void on_audio_customContextMenuRequested(const QPoint &pos);

    void on_MessageWidget_customContextMenuRequested(const QPoint &pos);

    void checkResUpdated();

    void on_file_customContextMenuRequested(const QPoint &pos);

    void contextMenuProc(QAction *action);

private:
    QString userID = "", iPath = "", vPath = "", aPath = "", fPath = "", mID = "", rID = "";
    bool iPathExist = false, vPathExist = false, aPathExist = false, fPathExist = false;
    Ui::MessageWidget *ui;
    QMediaPlayer *audioPlayer = nullptr;
    bool audio_played = false;
    QMediaPlayer *videoPlayer = nullptr;
    bool video_played = false;
    RoomInfoPanel *senderProfile = nullptr;
    QMenu *contextMenu = nullptr;
    char contextMenuParent;
    MessageViewerWindow *replyMes = nullptr;
};

#endif // MESSAGEWIDGET_H

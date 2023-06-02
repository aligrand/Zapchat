#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QWidget>
#include <QSqlQuery>
#include <QVBoxLayout>
#include <QInputDialog>
#include <QMenu>
#include <QFile>
#include <QFileDialog>
#include <QDir>

#include "../lib/server-connection/serverman.h"
#include "../lib/server-connection/sqlrecordqstring.h"
#include "../dependency/messagesdisplayer.h"
#include "../lib/ini-proc/iniproc.h"
#include "accountsettingspanel.h"
#include "addroomwindow.h"
#include "appsettingspanel.h"
#include "enterroomwindow.h"
#include "roominfopanel.h"
#include "roomwidget.h"
#include "messageviewerwindow.h"
#include "emojitable.h"

extern ServerMan *server;
extern QString myUsername;
extern qint64 myMessageIndex;

namespace Ui {
class ChatWindow;
}

class ChatWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ChatWindow(QWidget *parent = nullptr);
    ~ChatWindow();

private slots:
    void updateRoomList(QString AdditionalInfo);

    void on_settingsButton_clicked();

    void on_search_le_textChanged(const QString &arg1);

    void on_profile_lable_linkActivated(const QString &link);

    void on_refoButton_clicked();

    void on_attachmentButton_clicked();

    void on_emojiButton_clicked();

    void on_sendButton_clicked();

    void on_pinButton_clicked();

    void openChat(QString roomId);

    void aeRoomWindow_closeProc();

    void on_moreButton_clicked();

    void buttonsProc(QAction *action);

    void emojiProc(QListWidgetItem *item);

    void updatePreferences();

private:
    void delRoomListItems();
    void printRoomsList();

private:
    Ui::ChatWindow *ui;
    MessagesDisplayer *md = nullptr;
    MessageViewerWindow *pinMessageViewerWindow = nullptr;
    bool is_mvw_open = false;
    AccountSettingsPanel *accsp = nullptr;
    bool is_accsp_open = false;
    AppSettingsPanel *appsp = nullptr;
    bool is_appsp_open = false;
    AddRoomWindow *arw = nullptr;
    EnterRoomWindow *erw = nullptr;
    char aerw_open = '0'; // 0:nothing --- a:add room --- e:enter room
    RoomInfoPanel *rip = nullptr;
    bool is_rip_open = false;
    EmojiTable *emoTable = nullptr;
    bool is_emoTable_open = false;
    QSqlQuery sqlQuery;
    QVBoxLayout roomPanelLayout;
    QMenu *menu = nullptr;
    QString replyID = "", text = "", imagePath = "", videoPath = "", audioPath = "", filePath = "";
    QString menuParent = "";
    QString room_id = "";
    bool is_typeof_room_user;
};

#endif // CHATWINDOW_H

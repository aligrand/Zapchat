#ifndef MESSAGEVIEWERWINDOW_H
#define MESSAGEVIEWERWINDOW_H

#include <QMainWindow>

#include "../dependency/messagesdisplayer.h"

class MessagesDisplayer;

namespace Ui {
class MessageViewerWindow;
}

class MessageViewerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MessageViewerWindow(QString queryCondition, QString room_id = "", QWidget *parent = nullptr);
    ~MessageViewerWindow();

private:
    Ui::MessageViewerWindow *ui;
    MessagesDisplayer *md;
};

#endif // MESSAGEVIEWERWINDOW_H

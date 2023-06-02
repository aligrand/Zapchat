#include "messageviewerwindow.h"
#include "ui_messageviewerwindow.h"

MessageViewerWindow::MessageViewerWindow(QString queryCondition, QString room_id, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MessageViewerWindow)
{
    ui->setupUi(this);

    md = new MessagesDisplayer(queryCondition, room_id, ui->centralwidget);
}

MessageViewerWindow::~MessageViewerWindow()
{
    delete ui;
    delete md;
}

#include "messageviewerwindow.h"
#include "ui_messageviewerwindow.h"

MessageViewerWindow::MessageViewerWindow(QStringList MessageIDlist, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MessageViewerWindow)
{
    ui->setupUi(this);
}

MessageViewerWindow::~MessageViewerWindow()
{
    delete ui;
}

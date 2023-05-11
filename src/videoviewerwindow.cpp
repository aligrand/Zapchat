#include "videoviewerwindow.h"
#include "ui_videoviewerwindow.h"

VideoViewerWindow::VideoViewerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::VideoViewerWindow)
{
    ui->setupUi(this);
}

VideoViewerWindow::~VideoViewerWindow()
{
    delete ui;
}

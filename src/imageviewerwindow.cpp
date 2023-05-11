#include "imageviewerwindow.h"
#include "ui_imageviewerwindow.h"

ImageViewerWindow::ImageViewerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ImageViewerWindow)
{
    ui->setupUi(this);
}

ImageViewerWindow::~ImageViewerWindow()
{
    delete ui;
}

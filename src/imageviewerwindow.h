#ifndef IMAGEVIEWERWINDOW_H
#define IMAGEVIEWERWINDOW_H

#include <QMainWindow>

namespace Ui {
class ImageViewerWindow;
}

class ImageViewerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ImageViewerWindow(QWidget *parent = nullptr);
    ~ImageViewerWindow();

private:
    Ui::ImageViewerWindow *ui;
};

#endif // IMAGEVIEWERWINDOW_H

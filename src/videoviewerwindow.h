#ifndef VIDEOVIEWERWINDOW_H
#define VIDEOVIEWERWINDOW_H

#include <QMainWindow>

namespace Ui {
class VideoViewerWindow;
}

class VideoViewerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit VideoViewerWindow(QWidget *parent = nullptr);
    ~VideoViewerWindow();

private:
    Ui::VideoViewerWindow *ui;
};

#endif // VIDEOVIEWERWINDOW_H

#ifndef MESSAGEVIEWERWINDOW_H
#define MESSAGEVIEWERWINDOW_H

#include <QMainWindow>

namespace Ui {
class MessageViewerWindow;
}

class MessageViewerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MessageViewerWindow(QWidget *parent = nullptr);
    ~MessageViewerWindow();

private:
    Ui::MessageViewerWindow *ui;
};

#endif // MESSAGEVIEWERWINDOW_H

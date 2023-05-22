#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QWidget>

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
    void updateRoomList();

private:
    Ui::ChatWindow *ui;
};

#endif // CHATWINDOW_H

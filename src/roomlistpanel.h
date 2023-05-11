#ifndef ROOMLISTPANEL_H
#define ROOMLISTPANEL_H

#include <QWidget>

namespace Ui {
class RoomListPanel;
}

class RoomListPanel : public QWidget
{
    Q_OBJECT

public:
    explicit RoomListPanel(QWidget *parent = nullptr);
    ~RoomListPanel();

private:
    Ui::RoomListPanel *ui;
};

#endif // ROOMLISTPANEL_H

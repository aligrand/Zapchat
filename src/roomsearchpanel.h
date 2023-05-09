#ifndef ROOMSEARCHPANEL_H
#define ROOMSEARCHPANEL_H

#include <QWidget>

namespace Ui {
class RoomSearchPanel;
}

class RoomSearchPanel : public QWidget
{
    Q_OBJECT

public:
    explicit RoomSearchPanel(QWidget *parent = nullptr);
    ~RoomSearchPanel();

private:
    Ui::RoomSearchPanel *ui;
};

#endif // ROOMSEARCHPANEL_H

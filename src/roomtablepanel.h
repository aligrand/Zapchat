#ifndef ROOMTABLEPANEL_H
#define ROOMTABLEPANEL_H

#include <QWidget>

namespace Ui {
class RoomTablePanel;
}

class RoomTablePanel : public QWidget
{
    Q_OBJECT

public:
    explicit RoomTablePanel(QWidget *parent = nullptr);
    ~RoomTablePanel();

private:
    Ui::RoomTablePanel *ui;
};

#endif // ROOMTABLEPANEL_H

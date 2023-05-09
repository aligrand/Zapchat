#ifndef ROOMINFOPANEL_H
#define ROOMINFOPANEL_H

#include <QWidget>

namespace Ui {
class RoomInfoPanel;
}

class RoomInfoPanel : public QWidget
{
    Q_OBJECT

public:
    explicit RoomInfoPanel(QWidget *parent = nullptr);
    ~RoomInfoPanel();

private:
    Ui::RoomInfoPanel *ui;
};

#endif // ROOMINFOPANEL_H

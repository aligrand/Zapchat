#ifndef ROOMINFOPANEL_H
#define ROOMINFOPANEL_H

#include <QWidget>
#include <QString>
#include <QSqlQuery>

extern QString myUsername;

namespace Ui {
class RoomInfoPanel;
}

class RoomInfoPanel : public QWidget
{
    Q_OBJECT

public:
    // type -> 0:group 1:private 2:user
    explicit RoomInfoPanel(int type, QString id, QWidget *parent = nullptr);
    ~RoomInfoPanel();

private:
    Ui::RoomInfoPanel *ui;
};

#endif // ROOMINFOPANEL_H

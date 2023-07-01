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
    explicit RoomInfoPanel(bool isUser, QString id, QWidget *parent = nullptr);
    ~RoomInfoPanel();

private:
    Ui::RoomInfoPanel *ui;
};

#endif // ROOMINFOPANEL_H

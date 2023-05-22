#ifndef ROOMWIDGET_H
#define ROOMWIDGET_H

#include <QWidget>

namespace Ui {
class RoomWidget;
}

class RoomWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RoomWidget(QWidget *parent = nullptr);
    ~RoomWidget();

private:
    Ui::RoomWidget *ui;
};

#endif // ROOMWIDGET_H

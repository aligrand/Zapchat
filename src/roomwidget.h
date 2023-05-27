#ifndef ROOMWIDGET_H
#define ROOMWIDGET_H

#include <QWidget>
#include <QSqlQuery>
#include <QPixmap>
#include <QLabel>

#include "lib/server-connection/serverman.h"

extern ServerMan *server;
extern QString myUsername;

namespace Ui {
class RoomWidget;
}

class RoomWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RoomWidget(QString roomID, QWidget *parent = nullptr);
    ~RoomWidget();

signals:
    void clicked();

private slots:
    void newMessagesCame();
    void clickedProc();

private:
    Ui::RoomWidget *ui;
    QString rID = "";
    QLabel *newMCount = nullptr;
};

#endif // ROOMWIDGET_H

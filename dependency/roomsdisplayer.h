#ifndef ROOMSDISPLAYER_H
#define ROOMSDISPLAYER_H

#include <QWidget>

namespace Ui {
class RoomsDisplayer;
}

class RoomsDisplayer : public QWidget
{
    Q_OBJECT

public:
    explicit RoomsDisplayer(QWidget *parent = nullptr);
    ~RoomsDisplayer();

private:
    Ui::RoomsDisplayer *ui;
};

#endif // ROOMSDISPLAYER_H

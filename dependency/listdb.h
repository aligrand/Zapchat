#ifndef LISTDB_H
#define LISTDB_H

#include <QWidget>

namespace Ui {
class ListDB;
}

class ListDB : public QWidget
{
    Q_OBJECT

public:
    explicit ListDB(QWidget *parent = nullptr);
    ~ListDB();

private:
    Ui::ListDB *ui;
};

#endif // LISTDB_H

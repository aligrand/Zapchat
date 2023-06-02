#ifndef EMOJITABLE_H
#define EMOJITABLE_H

#include <QWidget>
#include <QFile>
#include <QListWidgetItem>

namespace Ui {
class EmojiTable;
}

class EmojiTable : public QWidget
{
    Q_OBJECT

public:
    explicit EmojiTable(QWidget *parent = nullptr);
    ~EmojiTable();

signals:
    void _itemClicked(QListWidgetItem *item);

private slots:
    void _itemClickedProc(QListWidgetItem *item);

private:
    Ui::EmojiTable *ui;
};

#endif // EMOJITABLE_H

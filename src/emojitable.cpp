#include "emojitable.h"
#include "ui_emojitable.h"

EmojiTable::EmojiTable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EmojiTable)
{
    ui->setupUi(this);
    connect(ui->listWidget, &QListWidget::itemClicked, this, &EmojiTable::_itemClickedProc);

    QListWidgetItem *item;
    QFont tempFont;
    QFile emojiFile("../res/emoji_dataset.txt");
    emojiFile.open(QIODevice::ReadOnly | QIODevice::Text);

    QString emoji;
    while (!emojiFile.atEnd())
    {
        emoji = emojiFile.readLine().trimmed();

        item = new QListWidgetItem(emoji);
        item->setTextAlignment(Qt::AlignHCenter);
        tempFont = item->font();
        tempFont.setPixelSize(25);
        item->setFont(tempFont);
        ui->listWidget->addItem(item);
    }

    emojiFile.close();
}

EmojiTable::~EmojiTable()
{
    delete ui;
}

void EmojiTable::_itemClickedProc(QListWidgetItem *item)
{
    emit _itemClicked(item);
}

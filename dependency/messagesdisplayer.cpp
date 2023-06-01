#include "messagesdisplayer.h"
#include "ui_messagesdisplayer.h"

MessagesDisplayer::MessagesDisplayer(QString room_id, QString queryCondition, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MessagesDisplayer),
    rid(room_id),
    qc(queryCondition)
{
    ui->setupUi(this);

    connect(server, &ServerMan::databaseUpdated, this, &MessagesDisplayer::updateMessagesQuery);

    contentLayout->setMargin(0);
    ui->scrollAreaWidgetContents->setLayout(contentLayout);

    ui->scrollArea->verticalScrollBar()->setSingleStep(1);

    sqlQuery.prepare("SELECT * FROM ? WHERE ?");
    sqlQuery.addBindValue(rid);
    sqlQuery.addBindValue(qc);
    sqlQuery.exec();

    sqlQuery.last();

    for (int i = 0; i < 30; ++i)
    {
        QHBoxLayout *tmpLayout = new QHBoxLayout;

        QBoxLayout::Direction direction = (sqlQuery.value("userID").toString() == myUsername)
                ? QBoxLayout::Direction::RightToLeft : QBoxLayout::Direction::LeftToRight;

        tmpLayout->addWidget(new MessageWidget(sqlQuery.value("id").toString()));
        tmpLayout->setDirection(direction);

        messagesList.push_back(tmpLayout);

        int tmpValue = ui->scrollArea->verticalScrollBar()->value();

        contentLayout->insertLayout(0, tmpLayout);
        ui->scrollArea->verticalScrollBar()->setValue(tmpValue + 1);

        sqlQuery.previous();
    }

    connect(ui->scrollArea->verticalScrollBar(), &QScrollBar::valueChanged,
            this, &MessagesDisplayer::loadMessage);
}

MessagesDisplayer::~MessagesDisplayer()
{
    delete ui;
}

void MessagesDisplayer::updateMessagesQuery(QString additionalInfo)
{
    if (additionalInfo == ("messages-" + rid + "-remove"))
    {
        emit recreateList();
    }
    else if (additionalInfo == ("messages-" + rid))
    {
        sqlQuery.prepare("SELECT * FROM ? WHERE ?");
        sqlQuery.addBindValue(rid);
        sqlQuery.addBindValue(qc);
        sqlQuery.exec();

        sqlQuery.seek(sqlQuery.at());
    }
}

void MessagesDisplayer::loadMessage(int value)
{
    if(value > lastVScrollBarValue && value - lastVScrollBarValue < 10)
    {
        if (sqlQuery.seek(sqlQuery.at() + 30) == false)
        {
            return;
        }

        QHBoxLayout *tmpLayout = new QHBoxLayout;

        QBoxLayout::Direction direction = (sqlQuery.value("userID").toString() == myUsername)
                ? QBoxLayout::Direction::RightToLeft : QBoxLayout::Direction::LeftToRight;

        tmpLayout->addWidget(new MessageWidget(sqlQuery.value("id").toString()));
        tmpLayout->setDirection(direction);

        messagesList.push_front(tmpLayout);
        delListItem(messagesList.back());
        messagesList.pop_back();

        contentLayout->addLayout(tmpLayout);

        sqlQuery.seek(sqlQuery.at() - 29);
    }
    else if (value < lastVScrollBarValue && lastVScrollBarValue - value < 10)
    {
        if (sqlQuery.seek(sqlQuery.at() - 1) == false)
        {
            return;
        }

        QHBoxLayout *tmpLayout = new QHBoxLayout;

        QBoxLayout::Direction direction = (sqlQuery.value("userID").toString() == myUsername)
                ? QBoxLayout::Direction::RightToLeft : QBoxLayout::Direction::LeftToRight;

        tmpLayout->addWidget(new MessageWidget(sqlQuery.value("id").toString()));
        tmpLayout->setDirection(direction);

        messagesList.push_back(tmpLayout);
        delListItem(messagesList.front());
        messagesList.pop_front();

        int tmpValue = ui->scrollArea->verticalScrollBar()->value();

        contentLayout->insertLayout(0, tmpLayout);
        ui->scrollArea->verticalScrollBar()->setValue(tmpValue + 1);
    }
}

void MessagesDisplayer::delListItem(QHBoxLayout *layout)
{
    layout->
}

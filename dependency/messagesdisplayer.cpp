#include "messagesdisplayer.h"
#include "ui_messagesdisplayer.h"

MessagesDisplayer::MessagesDisplayer(QString queryCondition, QString room_id, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MessagesDisplayer),
    rid(room_id),
    qc(queryCondition)
{
    ui->setupUi(this);

    contentLayout->setMargin(0);
    contentLayout = qobject_cast<QVBoxLayout *>(ui->scrollAreaWidgetContents->layout());

    connect(server, &ServerMan::databaseUpdated, this, &MessagesDisplayer::updateMessagesQuery);

    ui->scrollArea->verticalScrollBar()->setSingleStep(5);
    ui->scrollArea->verticalScrollBar()->setVisible(false);

    sqlQuery.prepare("SELECT * FROM messages WHERE " + qc);
    sqlQuery.exec();
    if (sqlQuery.last())
    {
        downAt = sqlQuery.at();

        for (int i = 0; i < 20; ++i)
        {
            QHBoxLayout *tmpLayout = new QHBoxLayout;
            QBoxLayout::Direction direction;
            MessageWidget *mw;
            QString css;

            if (sqlQuery.value("userID").toString() == myUsername)
            {
                direction = QBoxLayout::Direction::RightToLeft;
                css = "background: #9fcd6e;";
            }
            else
            {
                direction = QBoxLayout::Direction::LeftToRight;
                css = "background: #fff;";
            }

            mw = new MessageWidget(sqlQuery.value("id").toString());
            mw->setStyleSheet(css);

            tmpLayout->insertWidget(0, mw);
            tmpLayout->setDirection(direction);
            tmpLayout->addStretch();

            messagesList.push_back(tmpLayout);

            contentLayout->insertLayout(1, tmpLayout);

            upAt = sqlQuery.at();

            if (sqlQuery.previous() == false)
            {
                break;
            }
        }

        QTimer::singleShot(50, this, &MessagesDisplayer::scrollbarMax);

        connect(ui->scrollArea->verticalScrollBar(), &QScrollBar::valueChanged,
                this, &MessagesDisplayer::loadMessage);
    }
}

MessagesDisplayer::~MessagesDisplayer()
{
    for (int i = messagesList.size() - 1; i >= 0; --i)
    {
        delListItem(messagesList[i]);
    }

    delete ui;
    delete contentLayout;
}

void MessagesDisplayer::updateMessagesQuery(QString additionalInfo)
{
    if (additionalInfo == "messages-" + rid)
    {
        sqlQuery.prepare("SELECT * FROM messages WHERE " + qc);
        sqlQuery.exec();

        if (messagesList.size() < 20)
        {

            QHBoxLayout *tmpLayout = new QHBoxLayout;
            QBoxLayout::Direction direction;
            MessageWidget *mw;
            QString css;
            int height;

            if (!sqlQuery.seek(downAt + 1))
            {
                delete tmpLayout;
                return;
            }

            if (sqlQuery.value("userID").toString() == myUsername)
            {
                direction = QBoxLayout::Direction::RightToLeft;
                css = "background: #9fcd6e;";
            }
            else
            {
                direction = QBoxLayout::Direction::LeftToRight;
                css = "background: #fff;";
            }

            mw = new MessageWidget(sqlQuery.value("id").toString());
            mw->setStyleSheet(css);
            height = mw->sizeHint().height();

            tmpLayout->insertWidget(0, mw);
            tmpLayout->setDirection(direction);
            tmpLayout->addStretch();

            messagesList.push_front(tmpLayout);

            contentLayout->insertLayout(messagesList.size() + 1, tmpLayout);

            ++downAt;

            qDebug() << height;
            lastVScrollBarValue = ui->scrollArea->verticalScrollBar()->value() - height;
            ui->scrollArea->verticalScrollBar()->setValue(lastVScrollBarValue);
        }
    }
}


void MessagesDisplayer::loadMessage(int value)
{
    QHBoxLayout *tmpLayout = new QHBoxLayout;
    QBoxLayout::Direction direction;
    MessageWidget *mw;
    QString css;
    int height = 0;

    // go down
    if(value > lastVScrollBarValue && ui->scrollArea->verticalScrollBar()->maximum() - value < 50)
    {
        if (!sqlQuery.seek(downAt + 1))
        {
            lastVScrollBarValue = ui->scrollArea->verticalScrollBar()->value();

            delete tmpLayout;
            return;
        }

        if (sqlQuery.value("userID").toString() == myUsername)
        {
            direction = QBoxLayout::Direction::RightToLeft;
            css = "background: #9fcd6e;";
        }
        else
        {
            direction = QBoxLayout::Direction::LeftToRight;
            css = "background: #fff;";
        }

        qDebug() << sqlQuery.value("id").toString();

        mw = new MessageWidget(sqlQuery.value("id").toString());
        mw->setStyleSheet(css);
        height = mw->sizeHint().height();

        tmpLayout->insertWidget(0, mw);
        tmpLayout->setDirection(direction);
        tmpLayout->addStretch();

        delListItem(messagesList.last());
        messagesList.pop_back();
        messagesList.push_front(tmpLayout);

        contentLayout->insertLayout(messagesList.size() + 1, tmpLayout);

        ++downAt;
        ++upAt;

        qDebug() << height;
        lastVScrollBarValue = ui->scrollArea->verticalScrollBar()->value() - height;
    }
    // go up
    else if (value < lastVScrollBarValue && value - ui->scrollArea->verticalScrollBar()->minimum() < 50)
    {
        if (!sqlQuery.seek(upAt - 1))
        {
            lastVScrollBarValue = ui->scrollArea->verticalScrollBar()->value();

            delete tmpLayout;
            return;
        }

        if (sqlQuery.value("userID").toString() == myUsername)
        {
            direction = QBoxLayout::Direction::RightToLeft;
            css = "background: #9fcd6e;";
        }
        else
        {
            direction = QBoxLayout::Direction::LeftToRight;
            css = "background: #fff;";
        }

        qDebug() << sqlQuery.value("id").toString();

        mw = new MessageWidget(sqlQuery.value("id").toString());
        mw->setStyleSheet(css);
        height = mw->sizeHint().height();

        tmpLayout->insertWidget(0, mw);
        tmpLayout->setDirection(direction);
        tmpLayout->addStretch();

        delListItem(messagesList.front());
        messagesList.pop_front();
        messagesList.push_back(tmpLayout);

        contentLayout->insertLayout(1, tmpLayout);

        --downAt;
        --upAt;

        qDebug() << height;
        lastVScrollBarValue = ui->scrollArea->verticalScrollBar()->value() + height;
    }
    else
    {
        lastVScrollBarValue = ui->scrollArea->verticalScrollBar()->value();

        return;
    }

    ui->scrollArea->verticalScrollBar()->setValue(lastVScrollBarValue);
}

void MessagesDisplayer::delListItem(QHBoxLayout *layout)
{
    layout->takeAt(0)->widget()->deleteLater();
    delete layout->takeAt(0);
    delete layout;
}

void MessagesDisplayer::scrollbarMax()
{
    lastVScrollBarValue = ui->scrollArea->verticalScrollBar()->maximum();
    ui->scrollArea->verticalScrollBar()->setValue(lastVScrollBarValue);
}

#include "messagewidget.h"
#include "ui_messagewidget.h"

MessageWidget::MessageWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MessageWidget)
{
    ui->setupUi(this);
}

MessageWidget::~MessageWidget()
{
    delete ui;
}

void MessageWidget::on_replyButton_clicked()
{

}

void MessageWidget::on_horizontalSlider_sliderMoved(int position)
{

}

void MessageWidget::on_profilePicLable_linkActivated(const QString &link)
{

}

void MessageWidget::on_audioPlayPushButton_clicked()
{

}

void MessageWidget::on_fileDownloadPushButton_clicked()
{

}

void MessageWidget::on_imageLable_linkActivated(const QString &link)
{

}

void MessageWidget::on_imageLable_customContextMenuRequested(const QPoint &pos)
{

}

void MessageWidget::on_video_customContextMenuRequested(const QPoint &pos)
{

}

void MessageWidget::on_audio_customContextMenuRequested(const QPoint &pos)
{

}

void MessageWidget::on_MessageWidget_customContextMenuRequested(const QPoint &pos)
{

}

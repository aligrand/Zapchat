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

void MessageWidget::on_forwardutton_clicked()
{

}

void MessageWidget::on_replyButton_clicked()
{

}

void MessageWidget::on_pushButton_clicked()
{

}

void MessageWidget::on_horizontalSlider_sliderMoved(int position)
{

}

void MessageWidget::on_pushButton_2_clicked()
{

}

void MessageWidget::on_senderNameLable_linkActivated(const QString &link)
{

}

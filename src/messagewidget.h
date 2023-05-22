#ifndef MESSAGEWIDGET_H
#define MESSAGEWIDGET_H

#include <QWidget>

namespace Ui {
class MessageWidget;
}

class MessageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MessageWidget(QWidget *parent = nullptr);
    ~MessageWidget();

private slots:
    void on_forwardutton_clicked();

    void on_replyButton_clicked();

    void on_pushButton_clicked();

    void on_horizontalSlider_sliderMoved(int position);

    void on_pushButton_2_clicked();

    void on_senderNameLable_linkActivated(const QString &link);

private:
    Ui::MessageWidget *ui;
};

#endif // MESSAGEWIDGET_H

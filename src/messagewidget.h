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
    void on_replyButton_clicked();

    void on_horizontalSlider_sliderMoved(int position);

    void on_profilePicLable_linkActivated(const QString &link);

    void on_audioPlayPushButton_clicked();

    void on_fileDownloadPushButton_clicked();

    void on_imageLable_linkActivated(const QString &link);

    void on_imageLable_customContextMenuRequested(const QPoint &pos);

    void on_video_customContextMenuRequested(const QPoint &pos);

    void on_audio_customContextMenuRequested(const QPoint &pos);

    void on_MessageWidget_customContextMenuRequested(const QPoint &pos);

private:
    Ui::MessageWidget *ui;
};

#endif // MESSAGEWIDGET_H

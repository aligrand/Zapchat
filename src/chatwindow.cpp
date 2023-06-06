#include "chatwindow.h"
#include "ui_chatwindow.h"

ChatWindow::ChatWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatWindow)
{
    ui->setupUi(this);

    connect(server, &ServerMan::databaseUpdated, this, &ChatWindow::updateRoomList);

    QSizePolicy _size_policy = ui->chat_panel->sizePolicy();
    _size_policy.setRetainSizeWhenHidden(true);
    ui->chat_panel->setSizePolicy(_size_policy);

    ui->chat_panel->setVisible(false);

    ui->leftPanel->setDirection(QHBoxLayout::RightToLeft);
    ui->rightPanel->setDirection(QHBoxLayout::LeftToRight);

    IniProc ini("settings.ini");
    ui->messagesPanel->setStyleSheet("background-image: url(" + ini["chat-bg-image"] + ");");

    printRoomsList();
}

ChatWindow::~ChatWindow()
{
    delete ui;
}

void ChatWindow::updateRoomList(QString AdditionalInfo)
{
    if (AdditionalInfo == "rooms")
    {
        delRoomListItems();

        printRoomsList();
    }
}

void ChatWindow::on_settingsButton_clicked()
{
    delete menu;
    if (menuParent == "sb")
    {
        return;
    }
    menu = new QMenu;
    menuParent = "sb";
    menu->addAction(QIcon("../res/icon/acc_settings_icon.png"), "Account Settings");
    menu->addAction(QIcon("../res/icon/app_settings_icon.png"), "App Settings");
    menu->addAction(QIcon("../res/icon/add_room_icon.png"), "Add Room");
    menu->addAction(QIcon("../res/icon/enter_room_icon.png"), "Enter Room");
    menu->addAction(QIcon("../res/icon/remove_account_icon.png"), "Delete Account");
    menu->addAction(QIcon("../res/icon/leave_icon.png"), "Logout");
    connect(menu, &QMenu::triggered, this, &ChatWindow::buttonsProc);
    menu->popup(QPoint(ui->settingsButton->pos().x(), ui->settingsButton->pos().y() + 70));
}

void ChatWindow::on_search_le_textChanged(const QString &arg1)
{
    if (arg1 == "")
    {
        delRoomListItems();
        printRoomsList();
    }

    delRoomListItems();

    printRoomsList("roomID LIKE %" + arg1 + "% OR name LIKE %" + arg1 + "%");
}

void ChatWindow::on_profile_lable_linkActivated(const QString &link)
{
    if (is_rip_open)
    {
        rip->deleteLater();
        is_rip_open = false;
    }
    else
    {
        rip = new RoomInfoPanel(is_typeof_room_user, room_id);

        ui->rightPanel->addWidget(rip);

        is_rip_open = true;
    }
}

void ChatWindow::on_refoButton_clicked()
{
    replyID = QInputDialog::getText(this, "Zapchat", "Enter Message ID");

    QString tmp = "<html>"
                  "<head/>"
                  "<body>"
                  "<p><span style=\" font-weight:600;\">Reply:</span> &quot%1&quot;</p>"
                  "</body>"
                  "</html>";
    tmp = tmp.arg(replyID);

    ui->refoButton->setToolTip(tmp);
}

void ChatWindow::on_attachmentButton_clicked()
{
    delete menu;
    if (menuParent == "ab")
    {
        return;
    }
    menu = new QMenu;
    menuParent = "ab";
    menu->addAction(QIcon("../res/icon/image_icon.png"), "Image");
    menu->addAction("Image-clear");
    menu->addAction(QIcon("../res/icon/video_icon.png"), "Video");
    menu->addAction("video-clear");
    menu->addAction(QIcon("../res/icon/audio_icon.png"), "Audio");
    menu->addAction("Audio-clear");
    menu->addAction(QIcon("../res/icon/file_icon.png"), "File");
    menu->addAction("File-clear");
    connect(menu, &QMenu::triggered, this, &ChatWindow::buttonsProc);
    menu->popup(QPoint(ui->attachmentButton->pos().x(), ui->settingsButton->pos().y() - 70));
}

void ChatWindow::on_emojiButton_clicked()
{
    if (is_emoTable_open)
    {
        emoTable->deleteLater();

        is_emoTable_open = false;
    }

    emoTable = new EmojiTable();

    emoTable->setGeometry(ui->emojiButton->pos().x(), ui->emojiButton->pos().y() - 250,
                          100, 200);
    connect(emoTable, &EmojiTable::_itemClicked, this, &ChatWindow::emojiProc);

    emoTable->show();

    is_emoTable_open = true;
}

void ChatWindow::on_sendButton_clicked()
{   
    SqlRecordQString record;

    if (imagePath != "")
    {
        QFile::copy(imagePath, "Images/" + room_id + "-" + myUsername + "-" +
                    QString::number(myMessageIndex) + "I." + imagePath.split(".").last());
        imagePath = room_id + "-" + myUsername + "-" + QString::number(myMessageIndex) + "I." + imagePath.split(".").last();

        emit server->command("_UPLOAD_ Images/" + imagePath);
    }
    if (videoPath != "")
    {
        QFile::copy(videoPath, "Images/" + room_id + "-" + myUsername + "-" +
                    QString::number(myMessageIndex) + "V." + videoPath.split(".").last());
        videoPath = room_id + "-" + myUsername + "-" + QString::number(myMessageIndex) + "I." + videoPath.split(".").last();

        emit server->command("_UPLOAD_ Videos/" + videoPath);
    }
    if (audioPath != "")
    {
        QFile::copy(audioPath, "Images/" + room_id + "-" + myUsername + "-" +
                    QString::number(myMessageIndex) + "A." + audioPath.split(".").last());
        audioPath = room_id + "-" + myUsername + "-" + QString::number(myMessageIndex) + "I." + audioPath.split(".").last();

        emit server->command("_UPLOAD_ Audios/" + audioPath);
    }
    if (filePath != "")
    {
        QFile::copy(filePath, "Images/" + room_id + "-" + myUsername + "-" +
                    QString::number(myMessageIndex) + "F." + filePath.split(".").last());
        filePath = room_id + "-" + myUsername + "-" + QString::number(myMessageIndex) + "I." + filePath.split(".").last();

        emit server->command("_UPLOAD_ Files/" + filePath);
    }

    sqlQuery.prepare("INSERT INTO messages (id, roomID, userID, key, DT, replyID, text, imageADDRESS,"
                     "videoADDRESS, audioADDRESS, fileADDRESSS) VALUES (?, ?, ?, ?, date(), ?, ?, ?, ?, ?, ?)");
    sqlQuery.addBindValue(room_id + "-" + myUsername + "-" + QString::number(myMessageIndex));
    sqlQuery.addBindValue(room_id);
    sqlQuery.addBindValue(myUsername);
    sqlQuery.addBindValue(myMessageIndex);
    sqlQuery.addBindValue(replyID);
    sqlQuery.addBindValue(text);
    sqlQuery.addBindValue(imagePath);
    sqlQuery.addBindValue(videoPath);
    sqlQuery.addBindValue(audioPath);
    sqlQuery.addBindValue(filePath);
    sqlQuery.exec();

    sqlQuery.prepare("SELECT * FROM messages WHERE id = ?");
    sqlQuery.addBindValue(room_id + "-" + myUsername + "-" + QString::number(myMessageIndex));
    sqlQuery.exec();
    sqlQuery.first();

    record << sqlQuery.value("id").toString() << sqlQuery.value("roomID").toString() << sqlQuery.value("userID").toString()
           << sqlQuery.value("key").toString() << sqlQuery.value("DT").toString() << sqlQuery.value("replyID").toString()
           << sqlQuery.value("text").toString() << sqlQuery.value("imageADDRESS").toString() << sqlQuery.value("videoADDRESS").toString()
           << sqlQuery.value("audioADDRESS").toString() << sqlQuery.value("fileADDRESS").toString();
    record.end();

    ++myMessageIndex;

    emit server->command(QString("ADD-MESSAGE ") + record);

    ui->text_le->clear();

    ui->refoButton->setToolTip("<html>"
                               "<head/>"
                               "<body>"
                               "<p><span style=\" font-weight:600;\">Reply:</span> &quot;&quot;</p>"
                               "</body>"
                               "</html>");
    ui->attachmentButton->setToolTip("<html>"
                                     "<head/>"
                                     "<body>"
                                     "<p><span style=\" font-weight:600;\">Image: </span>&quot;&quot;</p>"
                                     "<p><span style=\" font-weight:600;\">Video: </span>&quot;&quot;</p>"
                                     "<p><span style=\" font-weight:600;\">Audio: </span>&quot;&quot;</p>"
                                     "<p><span style=\" font-weight:600;\">File: </span>&quot;&quot;</p>"
                                     "</body>"
                                     "</html>");
}

void ChatWindow::on_pinButton_clicked()
{
    delete menu;
    if (menuParent == "pb")
    {
        return;
    }
    menu = new QMenu;
    menuParent = "pb";
    menu->addAction("Open Pin");
    menu->addAction("Remove Pin");
    connect(menu, &QMenu::triggered, this, &ChatWindow::buttonsProc);
    menu->popup(QPoint(ui->pinButton->pos().x(), ui->settingsButton->pos().y() + 70));
}

void ChatWindow::openChat(QString roomId)
{
    room_id = roomId;

    ui->chat_panel->setVisible(true);

    md = new MessagesDisplayer("roomID=" + room_id, room_id, ui->messagesPanel);
    md->show();

    sqlQuery.prepare("SELECT * FROM rooms WHERE id=?");
    sqlQuery.addBindValue(room_id);
    sqlQuery.exec();
    sqlQuery.first();

    if (sqlQuery.value("type").toInt() == 0)
    {
        sqlQuery.prepare("SELECT * FROM participants WHERE roomID=? AND NOT userID=?");
        sqlQuery.addBindValue(room_id);
        sqlQuery.addBindValue(myUsername);
        sqlQuery.exec();
        sqlQuery.first();

        QString uid = sqlQuery.value("userID").toString();

        sqlQuery.prepare("SELECT * FROM users WHERE id=?");
        sqlQuery.addBindValue(uid);
        sqlQuery.exec();
        sqlQuery.first();

        ui->profile_lable->setText(sqlQuery.value("name").toString());
        ui->profile_pic_lable->setPixmap(QPixmap("Cache/" + sqlQuery.value("photoADDRESS").toString()));

        is_typeof_room_user = true;

    }
    else
    {
        ui->profile_lable->setText(sqlQuery.value("name").toString());
        ui->profile_pic_lable->setPixmap(QPixmap("Cache/" + sqlQuery.value("photoADDRESS").toString()));

        is_typeof_room_user = false;
    }
}

void ChatWindow::aeRoomWindow_closeProc()
{
    aerw_open = '0';
}

void ChatWindow::delRoomListItems()
{
    QLayoutItem *item;
    while ((item = roomPanelLayout.takeAt(0)) != nullptr)
    {
        item->widget()->deleteLater();
        delete item;
    }
}

void ChatWindow::printRoomsList(QString additionalInfo)
{
    roomPanelLayout.addStretch();
    ui->roomPanelContents->setLayout(&roomPanelLayout);

    sqlQuery.prepare("SELECT id FROM rooms WHERE ?");
    sqlQuery.addBindValue(additionalInfo);
    sqlQuery.exec();

    RoomWidget *rwTmp;
    while (true)
    {
        rwTmp = new RoomWidget(sqlQuery.value("id").toString());
        connect(rwTmp, &RoomWidget::clicked, this, &ChatWindow::openChat);

        roomPanelLayout.addWidget(rwTmp);
        if (sqlQuery.next() == false)
        {
            break;
        }
    }
}

void ChatWindow::on_moreButton_clicked()
{
    delete menu;
    if (menuParent == "mb")
    {
        return;
    }
    menu = new QMenu;
    menuParent = "mb";
    menu->addAction(QIcon("../res/icon/search_icon.png"), "Serch");
    menu->addAction(QIcon("../res/icon/leave_icon.png"), "Left");
    menu->addAction(QIcon("../res/icon/edit_room_icon.png"), "Edit Room");
    connect(menu, &QMenu::triggered, this, &ChatWindow::buttonsProc);
    menu->popup(QPoint(ui->pinButton->pos().x(), ui->settingsButton->pos().y() + 70));
}

void ChatWindow::buttonsProc(QAction *action)
{
    if (action->text() == "Account Settings")
    {
        if (is_accsp_open)
        {
            accsp->deleteLater();
            is_accsp_open = false;
        }
        else
        {
            accsp = new AccountSettingsPanel(true, myUsername);

            ui->leftPanel->addWidget(accsp);
            is_accsp_open = true;
        }
    }
    else if (action->text() == "App Settings")
    {
        if (is_appsp_open)
        {
            appsp->deleteLater();
            is_appsp_open = false;
        }
        else
        {
            appsp = new AppSettingsPanel();
            connect(appsp, &AppSettingsPanel::preferencesUpdated, this, &ChatWindow::updatePreferences);

            ui->leftPanel->addWidget(appsp);
            is_appsp_open = true;
        }
    }
    else if (action->text() == "Add Room")
    {
        if (aerw_open == '0')
        {
            arw = new AddRoomWindow();
            connect(arw, &AddRoomWindow::windowClosed, this, &ChatWindow::aeRoomWindow_closeProc);
            arw->show();
        }
    }
    else if (action->text() == "Enter Room")
    {
        if (aerw_open == '0')
        {
            erw = new EnterRoomWindow();
            connect(erw, &EnterRoomWindow::windowClosed, this, &ChatWindow::aeRoomWindow_closeProc);
            erw->show();
        }
    }
    else if (action->text() == "Delete Account")
    {
        emit server->command("REMOVE-USER " + myUsername);

        sqlQuery.prepare("DELETE FROM rooms");
        sqlQuery.exec();
        sqlQuery.prepare("DELETE FROM users");
        sqlQuery.exec();

        QFile::resize("job-queue.txt", 0);
        QFile::resize("message-index.txt", 0);
        QFile::resize("userinfo.txt", 0);

        QDir("Images").removeRecursively();
        QDir().mkdir("Images");
        QDir("Videos").removeRecursively();
        QDir().mkdir("Videos");
        QDir("Audios").removeRecursively();
        QDir().mkdir("Audios");
        QDir("Files").removeRecursively();
        QDir().mkdir("Files");
        QDir("Cache").removeRecursively();
        QDir().mkdir("Cache");

        exit(0);
    }
    else if (action->text() == "Logout")
    {
        sqlQuery.prepare("DELETE FROM rooms");
        sqlQuery.exec();
        sqlQuery.prepare("DELETE FROM users");
        sqlQuery.exec();

        QFile::resize("job-queue.txt", 0);
        QFile::resize("message-index.txt", 0);
        QFile::resize("userinfo.txt", 0);

        QDir("Images").removeRecursively();
        QDir().mkdir("Images");
        QDir("Videos").removeRecursively();
        QDir().mkdir("Videos");
        QDir("Audios").removeRecursively();
        QDir().mkdir("Audios");
        QDir("Files").removeRecursively();
        QDir().mkdir("Files");
        QDir("Cache").removeRecursively();
        QDir().mkdir("Cache");

        exit(0);
    }
    else if (action->text() == "Image")
    {
        imagePath = QFileDialog::getOpenFileName(this,
            tr("Open Image"), tr("Image Files (*.png *.jpg)"));

        if (imagePath == "")
        {
            QMessageBox::warning(this, "Warning", "No file selected");

            return;
        }

        QString tmp = "<html>"
                      "<head/>"
                      "<body>"
                      "<p><span style=\" font-weight:600;\">Image: </span>&quot;%1&quot;</p>"
                      "<p><span style=\" font-weight:600;\">Video: </span>&quot;%2&quot;</p>"
                      "<p><span style=\" font-weight:600;\">Audio: </span>&quot;%3&quot;</p>"
                      "<p><span style=\" font-weight:600;\">File: </span>&quot;%4&quot;</p>"
                      "</body>"
                      "</html>";
        tmp = tmp.arg(imagePath).arg(videoPath).arg(audioPath).arg(filePath);

        ui->attachmentButton->setToolTip(tmp);
    }
    else if (action->text() == "Image-clear")
    {
        imagePath = "";

        QString tmp = "<html>"
                      "<head/>"
                      "<body>"
                      "<p><span style=\" font-weight:600;\">Image: </span>&quot;%1&quot;</p>"
                      "<p><span style=\" font-weight:600;\">Video: </span>&quot;%2&quot;</p>"
                      "<p><span style=\" font-weight:600;\">Audio: </span>&quot;%3&quot;</p>"
                      "<p><span style=\" font-weight:600;\">File: </span>&quot;%4&quot;</p>"
                      "</body>"
                      "</html>";
        tmp = tmp.arg(imagePath).arg(videoPath).arg(audioPath).arg(filePath);

        ui->attachmentButton->setToolTip(tmp);
    }
    else if (action->text() == "Video")
    {
        videoPath = QFileDialog::getOpenFileName(this,
            tr("Open Video"), tr("Video Files (*.mp4 *.m4v)"));

        if (videoPath == "")
        {
            QMessageBox::warning(this, "Warning", "No file selected");

            return;
        }

        QString tmp = "<html>"
                      "<head/>"
                      "<body>"
                      "<p><span style=\" font-weight:600;\">Image: </span>&quot;%1&quot;</p>"
                      "<p><span style=\" font-weight:600;\">Video: </span>&quot;%2&quot;</p>"
                      "<p><span style=\" font-weight:600;\">Audio: </span>&quot;%3&quot;</p>"
                      "<p><span style=\" font-weight:600;\">File: </span>&quot;%4&quot;</p>"
                      "</body>"
                      "</html>";
        tmp = tmp.arg(imagePath).arg(videoPath).arg(audioPath).arg(filePath);

        ui->attachmentButton->setToolTip(tmp);
    }
    else if (action->text() == "Video-clear")
    {
        videoPath = "";

        QString tmp = "<html>"
                      "<head/>"
                      "<body>"
                      "<p><span style=\" font-weight:600;\">Image: </span>&quot;%1&quot;</p>"
                      "<p><span style=\" font-weight:600;\">Video: </span>&quot;%2&quot;</p>"
                      "<p><span style=\" font-weight:600;\">Audio: </span>&quot;%3&quot;</p>"
                      "<p><span style=\" font-weight:600;\">File: </span>&quot;%4&quot;</p>"
                      "</body>"
                      "</html>";
        tmp = tmp.arg(imagePath).arg(videoPath).arg(audioPath).arg(filePath);

        ui->attachmentButton->setToolTip(tmp);
    }
    else if (action->text() == "Audio")
    {
        audioPath = QFileDialog::getOpenFileName(this,
            tr("Open Audio"), tr("Audio Files (*.mp3 *.wav)"));

        if (audioPath == "")
        {
            QMessageBox::warning(this, "Warning", "No file selected");

            return;
        }

        QString tmp = "<html>"
                      "<head/>"
                      "<body>"
                      "<p><span style=\" font-weight:600;\">Image: </span>&quot;%1&quot;</p>"
                      "<p><span style=\" font-weight:600;\">Video: </span>&quot;%2&quot;</p>"
                      "<p><span style=\" font-weight:600;\">Audio: </span>&quot;%3&quot;</p>"
                      "<p><span style=\" font-weight:600;\">File: </span>&quot;%4&quot;</p>"
                      "</body>"
                      "</html>";
        tmp = tmp.arg(imagePath).arg(videoPath).arg(audioPath).arg(filePath);

        ui->attachmentButton->setToolTip(tmp);
    }
    else if (action->text() == "Audio-clear")
    {
        audioPath = "";

        QString tmp = "<html>"
                      "<head/>"
                      "<body>"
                      "<p><span style=\" font-weight:600;\">Image: </span>&quot;%1&quot;</p>"
                      "<p><span style=\" font-weight:600;\">Video: </span>&quot;%2&quot;</p>"
                      "<p><span style=\" font-weight:600;\">Audio: </span>&quot;%3&quot;</p>"
                      "<p><span style=\" font-weight:600;\">File: </span>&quot;%4&quot;</p>"
                      "</body>"
                      "</html>";
        tmp = tmp.arg(imagePath).arg(videoPath).arg(audioPath).arg(filePath);

        ui->attachmentButton->setToolTip(tmp);
    }
    else if (action->text() == "File")
    {
        filePath = QFileDialog::getOpenFileName(this,
            tr("Open File"), tr("All Files (*.*)"));

        if (filePath == "")
        {
            QMessageBox::warning(this, "Warning", "No file selected");

            return;
        }

        QString tmp = "<html>"
                      "<head/>"
                      "<body>"
                      "<p><span style=\" font-weight:600;\">Image: </span>&quot;%1&quot;</p>"
                      "<p><span style=\" font-weight:600;\">Video: </span>&quot;%2&quot;</p>"
                      "<p><span style=\" font-weight:600;\">Audio: </span>&quot;%3&quot;</p>"
                      "<p><span style=\" font-weight:600;\">File: </span>&quot;%4&quot;</p>"
                      "</body>"
                      "</html>";
        tmp = tmp.arg(imagePath).arg(videoPath).arg(audioPath).arg(filePath);

        ui->attachmentButton->setToolTip(tmp);
    }
    else if (action->text() == "File-clear")
    {
        filePath = "";

        QString tmp = "<html>"
                      "<head/>"
                      "<body>"
                      "<p><span style=\" font-weight:600;\">Image: </span>&quot;%1&quot;</p>"
                      "<p><span style=\" font-weight:600;\">Video: </span>&quot;%2&quot;</p>"
                      "<p><span style=\" font-weight:600;\">Audio: </span>&quot;%3&quot;</p>"
                      "<p><span style=\" font-weight:600;\">File: </span>&quot;%4&quot;</p>"
                      "</body>"
                      "</html>";
        tmp = tmp.arg(imagePath).arg(videoPath).arg(audioPath).arg(filePath);

        ui->attachmentButton->setToolTip(tmp);
    }
    else if (action->text() == "Open Pin")
    {
        sqlQuery.prepare("SELECT pin FROM rooms WHERE roomID=?");
        sqlQuery.addBindValue(room_id);
        sqlQuery.exec();
        sqlQuery.first();

        pinMessageViewerWindow = new MessageViewerWindow("id=" + sqlQuery.value("pin").toString());
    }
    else if (action->text() == "Remove Pin")
    {
        sqlQuery.prepare("UPDATE rooms SET pin='' WHERE id=?");
        sqlQuery.addBindValue(room_id);
        sqlQuery.exec();

        sqlQuery.prepare("SELECT * FROM rooms WHERE id=?");
        sqlQuery.addBindValue(room_id);
        sqlQuery.exec();
        sqlQuery.first();

        SqlRecordQString record;
        record << sqlQuery.value("id").toString() << sqlQuery.value("name").toString()
               << sqlQuery.value("photoADDRESS").toString() << sqlQuery.value("info").toString()
               << sqlQuery.value("type").toString() << sqlQuery.value("pin").toString();
        record.end();

        emit server->command("EDIT-ROOM " + QString(record));
    }
    else if (action->text() == "Search")
    {
        QString pattern = QInputDialog::getText(this, "Zapchat", "Enter your keyword");

        searchMessageViewerWindow = new MessageViewerWindow("text LIKE %" + pattern + "%");
    }
    else if (action->text() == "Left")
    {
        emit server->command("REMOVE-PARTICIPANT " + room_id + " " + myUsername);

        sqlQuery.prepare("DELETE FROM rooms WHERE roomID=?");
        sqlQuery.addBindValue(room_id);
        sqlQuery.exec();
    }
    else if (action->text() == "Edit Room")
    {
        if (is_roomsp_open)
        {
            roomsp->deleteLater();
            is_roomsp_open = false;
        }
        else
        {
            roomsp = new AccountSettingsPanel(false, myUsername);

            ui->leftPanel->addWidget(roomsp);
            is_roomsp_open = true;
        }
    }
}

void ChatWindow::emojiProc(QListWidgetItem *item)
{
    QString temp = ui->text_le->toPlainText();
    temp += item->text();
    ui->text_le->setText(temp);
}

void ChatWindow::updatePreferences()
{
    IniProc ini("settings.ini");
    ui->messagesPanel->setStyleSheet("background-image: url(" + ini["chat-bg-image"] + ");");
}

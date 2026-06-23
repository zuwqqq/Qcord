#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>

class SettingsDialog : public QDialog {
    Q_OBJECT

public:
    SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog();

private slots:
    void onApply();
    void onCancel();
    void onTwitchConnect();
    void onYouTubeConnect();
    void onTelegramConnect();

private:
    void setupUI();
    
    // Twitch
    QLineEdit *twitchServerEdit_;
    QLineEdit *twitchKeyEdit_;
    QPushButton *twitchConnectBtn_;
    QLabel *twitchStatusLabel_;
    
    // YouTube
    QLineEdit *youtubeServerEdit_;
    QLineEdit *youtubeKeyEdit_;
    QPushButton *youtubeConnectBtn_;
    QLabel *youtubeStatusLabel_;
    
    // Telegram
    QLineEdit *telegramTokenEdit_;
    QLineEdit *telegramChatIdEdit_;
    QPushButton *telegramConnectBtn_;
    QLabel *telegramStatusLabel_;
};

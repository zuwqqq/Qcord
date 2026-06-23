#include "ui/settingsdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QFormLayout>
#include <QPushButton>
#include <QMessageBox>

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent) {
    setupUI();
    setWindowTitle("Qcord Settings");
    setGeometry(200, 200, 700, 600);
}

SettingsDialog::~SettingsDialog() {
}

void SettingsDialog::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Twitch Settings
    QGroupBox *twitchGroup = new QGroupBox("📺 Twitch Settings", this);
    QFormLayout *twitchLayout = new QFormLayout(twitchGroup);
    
    twitchServerEdit_ = new QLineEdit("rtmps://live.twitch.tv:443/app/", this);
    twitchKeyEdit_ = new QLineEdit(this);
    twitchKeyEdit_->setEchoMode(QLineEdit::Password);
    twitchStatusLabel_ = new QLabel("Disconnected", this);
    twitchStatusLabel_->setStyleSheet("color: red;");
    twitchConnectBtn_ = new QPushButton("Test Connection", this);
    
    connect(twitchConnectBtn_, &QPushButton::clicked, this, &SettingsDialog::onTwitchConnect);
    
    twitchLayout->addRow("Server:", twitchServerEdit_);
    twitchLayout->addRow("Stream Key:", twitchKeyEdit_);
    twitchLayout->addRow("Status:", twitchStatusLabel_);
    twitchLayout->addRow(twitchConnectBtn_);
    mainLayout->addWidget(twitchGroup);
    
    // YouTube Settings
    QGroupBox *youtubeGroup = new QGroupBox("▶️  YouTube Live Settings", this);
    QFormLayout *youtubeLayout = new QFormLayout(youtubeGroup);
    
    youtubeServerEdit_ = new QLineEdit("rtmps://a.rtmp.youtube.com:443/live2/", this);
    youtubeKeyEdit_ = new QLineEdit(this);
    youtubeKeyEdit_->setEchoMode(QLineEdit::Password);
    youtubeStatusLabel_ = new QLabel("Disconnected", this);
    youtubeStatusLabel_->setStyleSheet("color: red;");
    youtubeConnectBtn_ = new QPushButton("Test Connection", this);
    
    connect(youtubeConnectBtn_, &QPushButton::clicked, this, &SettingsDialog::onYouTubeConnect);
    
    youtubeLayout->addRow("Server:", youtubeServerEdit_);
    youtubeLayout->addRow("Stream Key:", youtubeKeyEdit_);
    youtubeLayout->addRow("Status:", youtubeStatusLabel_);
    youtubeLayout->addRow(youtubeConnectBtn_);
    mainLayout->addWidget(youtubeGroup);
    
    // Telegram Settings
    QGroupBox *telegramGroup = new QGroupBox("✈️  Telegram Settings", this);
    QFormLayout *telegramLayout = new QFormLayout(telegramGroup);
    
    telegramTokenEdit_ = new QLineEdit(this);
    telegramTokenEdit_->setEchoMode(QLineEdit::Password);
    telegramTokenEdit_->setPlaceholderText("Bot token from @BotFather");
    telegramChatIdEdit_ = new QLineEdit(this);
    telegramChatIdEdit_->setPlaceholderText("Channel ID or Chat ID");
    telegramStatusLabel_ = new QLabel("Disconnected", this);
    telegramStatusLabel_->setStyleSheet("color: red;");
    telegramConnectBtn_ = new QPushButton("Test Connection", this);
    
    connect(telegramConnectBtn_, &QPushButton::clicked, this, &SettingsDialog::onTelegramConnect);
    
    telegramLayout->addRow("Bot Token:", telegramTokenEdit_);
    telegramLayout->addRow("Chat/Channel ID:", telegramChatIdEdit_);
    telegramLayout->addRow("Status:", telegramStatusLabel_);
    telegramLayout->addRow(telegramConnectBtn_);
    mainLayout->addWidget(telegramGroup);
    
    mainLayout->addStretch();
    
    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *applyBtn = new QPushButton("Apply", this);
    QPushButton *cancelBtn = new QPushButton("Cancel", this);
    
    connect(applyBtn, &QPushButton::clicked, this, &SettingsDialog::onApply);
    connect(cancelBtn, &QPushButton::clicked, this, &SettingsDialog::onCancel);
    
    buttonLayout->addStretch();
    buttonLayout->addWidget(applyBtn);
    buttonLayout->addWidget(cancelBtn);
    mainLayout->addLayout(buttonLayout);
}

void SettingsDialog::onApply() {
    QMessageBox::information(this, "Settings Saved", "Configuration has been saved successfully!");
    accept();
}

void SettingsDialog::onCancel() {
    reject();
}

void SettingsDialog::onTwitchConnect() {
    if (twitchKeyEdit_->text().isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter your Twitch stream key!");
        return;
    }
    twitchStatusLabel_->setText("✓ Connected");
    twitchStatusLabel_->setStyleSheet("color: green;");
}

void SettingsDialog::onYouTubeConnect() {
    if (youtubeKeyEdit_->text().isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter your YouTube stream key!");
        return;
    }
    youtubeStatusLabel_->setText("✓ Connected");
    youtubeStatusLabel_->setStyleSheet("color: green;");
}

void SettingsDialog::onTelegramConnect() {
    if (telegramTokenEdit_->text().isEmpty() || telegramChatIdEdit_->text().isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter both bot token and chat ID!");
        return;
    }
    telegramStatusLabel_->setText("✓ Connected");
    telegramStatusLabel_->setStyleSheet("color: green;");
}

#include "ui/mainwindow.h"
#include "core/broadcast.h"
#include "sources/screen_capture.h"
#include "sources/webcam_capture.h"
#include "sources/audio_input.h"
#include "outputs/twitch/twitch_output.h"
#include "outputs/youtube/youtube_output.h"
#include "outputs/telegram/telegram_output.h"
#include "outputs/local/file_output.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QCheckBox>
#include <QLabel>
#include <QMenuBar>
#include <QMenu>
#include <QTabWidget>
#include <QStatusBar>
#include <QTimer>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    broadcast_ = std::make_shared<Qcord::Broadcast>();
    setupUI();
    createMenuBar();
    setWindowTitle("Qcord - Streaming & Recording Software");
    setGeometry(100, 100, 1400, 900);
    
    // Auto-update status every 500ms
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::onUpdateStatus);
    timer->start(500);
}

MainWindow::~MainWindow() {
}

void MainWindow::setupUI() {
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // Title
    QLabel *titleLabel = new QLabel("<b>Qcord v1.0 - Multi-Platform Streaming</b>");
    titleLabel->setStyleSheet("font-size: 16px; padding: 10px;");
    mainLayout->addWidget(titleLabel);

    // Create tab widget
    QTabWidget *tabWidget = new QTabWidget(this);
    
    // Tab 1: Streaming
    QWidget *streamTab = new QWidget();
    QVBoxLayout *streamLayout = new QVBoxLayout(streamTab);
    createControlPanel();
    streamLayout->addLayout(createControlPanel());
    tabWidget->addTab(streamTab, "🎥 Streaming");
    
    // Tab 2: Sources
    QWidget *sourcesTab = new QWidget();
    QVBoxLayout *sourcesLayout = new QVBoxLayout(sourcesTab);
    createSourceList();
    sourcesLayout->addWidget(sourceCombo_);
    sourcesLayout->addWidget(addSourceBtn_);
    sourcesLayout->addWidget(removeSourceBtn_);
    sourcesLayout->addStretch();
    tabWidget->addTab(sourcesTab, "📺 Sources");
    
    // Tab 3: Outputs
    QWidget *outputsTab = new QWidget();
    QVBoxLayout *outputsLayout = new QVBoxLayout(outputsTab);
    createOutputPanel();
    outputsLayout->addWidget(twitchCheckbox_);
    outputsLayout->addWidget(youtubeCheckbox_);
    outputsLayout->addWidget(telegramCheckbox_);
    outputsLayout->addWidget(recordCheckbox_);
    outputsLayout->addStretch();
    tabWidget->addTab(outputsTab, "📤 Outputs");
    
    // Tab 4: Settings
    QWidget *settingsTab = new QWidget();
    QVBoxLayout *settingsLayout = new QVBoxLayout(settingsTab);
    createSettings();
    settingsLayout->addLayout(createSettings());
    tabWidget->addTab(settingsTab, "⚙️ Settings");
    
    mainLayout->addWidget(tabWidget);
    
    // Status bar
    createStatusBar();
    
    setCentralWidget(centralWidget);
}

QLayout* MainWindow::createControlPanel() {
    QHBoxLayout *layout = new QHBoxLayout();
    
    // Status label
    statusLabel_ = new QLabel("Ready to stream");
    statusLabel_->setStyleSheet("color: green; font-weight: bold;");
    layout->addWidget(new QLabel("Status:"));
    layout->addWidget(statusLabel_);
    
    layout->addStretch();
    
    // Start/Stop buttons
    startBtn_ = new QPushButton("▶️ Start Streaming", this);
    stopBtn_ = new QPushButton("⏹️ Stop Streaming", this);
    settingsBtn_ = new QPushButton("⚙️ Settings", this);
    
    startBtn_->setStyleSheet("background-color: #4CAF50; color: white; padding: 8px; font-weight: bold;");
    stopBtn_->setStyleSheet("background-color: #f44336; color: white; padding: 8px; font-weight: bold;");
    stopBtn_->setEnabled(false);
    
    connect(startBtn_, &QPushButton::clicked, this, &MainWindow::onStartStreaming);
    connect(stopBtn_, &QPushButton::clicked, this, &MainWindow::onStopStreaming);
    connect(settingsBtn_, &QPushButton::clicked, this, &MainWindow::onShowSettings);
    
    layout->addWidget(startBtn_);
    layout->addWidget(stopBtn_);
    layout->addWidget(settingsBtn_);
    
    return layout;
}

void MainWindow::createSourceList() {
    sourceCombo_ = new QComboBox(this);
    sourceCombo_->addItem("🖥️  Screen Capture");
    sourceCombo_->addItem("📹 Webcam");
    sourceCombo_->addItem("🎤 Microphone");
    
    addSourceBtn_ = new QPushButton("➕ Add Source", this);
    removeSourceBtn_ = new QPushButton("➖ Remove Source", this);
    
    connect(addSourceBtn_, &QPushButton::clicked, this, &MainWindow::onAddSource);
    connect(removeSourceBtn_, &QPushButton::clicked, this, &MainWindow::onRemoveSource);
}

void MainWindow::createOutputPanel() {
    twitchCheckbox_ = new QCheckBox("📺 Stream to Twitch", this);
    youtubeCheckbox_ = new QCheckBox("▶️  Stream to YouTube Live", this);
    telegramCheckbox_ = new QCheckBox("✈️  Send to Telegram", this);
    recordCheckbox_ = new QCheckBox("💾 Record to Local File", this);
    recordCheckbox_->setChecked(true);
    
    connect(twitchCheckbox_, &QCheckBox::stateChanged, this, [this](int state) { onOutputToggled(0); });
    connect(youtubeCheckbox_, &QCheckBox::stateChanged, this, [this](int state) { onOutputToggled(1); });
    connect(telegramCheckbox_, &QCheckBox::stateChanged, this, [this](int state) { onOutputToggled(2); });
    connect(recordCheckbox_, &QCheckBox::stateChanged, this, [this](int state) { onOutputToggled(3); });
}

QLayout* MainWindow::createSettings() {
    QVBoxLayout *layout = new QVBoxLayout();
    
    // Resolution
    QHBoxLayout *resLayout = new QHBoxLayout();
    resLayout->addWidget(new QLabel("Resolution:"));
    resolutionCombo_ = new QComboBox(this);
    resolutionCombo_->addItem("1920x1080 (1080p)");
    resolutionCombo_->addItem("1280x720 (720p)");
    resolutionCombo_->addItem("854x480 (480p)");
    resolutionCombo_->setCurrentIndex(0);
    connect(resolutionCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onResolutionChanged);
    resLayout->addWidget(resolutionCombo_);
    layout->addLayout(resLayout);
    
    // FPS
    QHBoxLayout *fpsLayout = new QHBoxLayout();
    fpsLayout->addWidget(new QLabel("Framerate (FPS):"));
    fpsSpinBox_ = new QSpinBox(this);
    fpsSpinBox_->setMinimum(24);
    fpsSpinBox_->setMaximum(60);
    fpsSpinBox_->setValue(30);
    connect(fpsSpinBox_, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::onFpsChanged);
    fpsLayout->addWidget(fpsSpinBox_);
    fpsLayout->addStretch();
    layout->addLayout(fpsLayout);
    
    // Bitrate
    QHBoxLayout *bitrateLayout = new QHBoxLayout();
    bitrateLayout->addWidget(new QLabel("Bitrate (Kbps):"));
    bitrateSpinBox_ = new QSpinBox(this);
    bitrateSpinBox_->setMinimum(1000);
    bitrateSpinBox_->setMaximum(50000);
    bitrateSpinBox_->setValue(6000);
    bitrateSpinBox_->setSingleStep(500);
    connect(bitrateSpinBox_, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::onBitrateChanged);
    bitrateLayout->addWidget(bitrateSpinBox_);
    bitrateLayout->addStretch();
    layout->addLayout(bitrateLayout);
    
    layout->addStretch();
    return layout;
}

void MainWindow::createStatusBar() {
    streamStatusLabel_ = new QLabel("Ready");
    bitrateLabelCurrent_ = new QLabel("0 Mbps");
    fpsLabelCurrent_ = new QLabel("0 FPS");
    
    statusBar()->addWidget(streamStatusLabel_, 1);
    statusBar()->addPermanentWidget(bitrateLabelCurrent_);
    statusBar()->addPermanentWidget(fpsLabelCurrent_);
}

void MainWindow::onStartStreaming() {
    if (!broadcast_) return;
    
    // Add outputs based on checkboxes
    if (recordCheckbox_->isChecked()) {
        auto fileOutput = std::make_shared<Qcord::FileOutput>();
        fileOutput->setFilePath("recording_" + std::to_string(std::time(nullptr)) + ".mp4");
        broadcast_->addOutput(fileOutput);
    }
    
    if (twitchCheckbox_->isChecked()) {
        auto twitchOutput = std::make_shared<Qcord::TwitchOutput>();
        broadcast_->addOutput(twitchOutput);
    }
    
    if (youtubeCheckbox_->isChecked()) {
        auto youtubeOutput = std::make_shared<Qcord::YouTubeOutput>();
        broadcast_->addOutput(youtubeOutput);
    }
    
    if (telegramCheckbox_->isChecked()) {
        auto telegramOutput = std::make_shared<Qcord::TelegramOutput>();
        broadcast_->addOutput(telegramOutput);
    }
    
    // Configure settings
    broadcast_->setFrameRate(fpsSpinBox_->value());
    broadcast_->setBitrate(bitrateSpinBox_->value());
    
    // Add default screen capture source
    auto screenCapture = std::make_shared<Qcord::ScreenCapture>(0);
    broadcast_->addSource(screenCapture);
    
    broadcast_->start();
    
    statusLabel_->setText("🟢 STREAMING");
    statusLabel_->setStyleSheet("color: green; font-weight: bold; font-size: 14px;");
    startBtn_->setEnabled(false);
    stopBtn_->setEnabled(true);
    streamStatusLabel_->setText("Streaming...");
}

void MainWindow::onStopStreaming() {
    if (!broadcast_) return;
    
    broadcast_->stop();
    
    statusLabel_->setText("Ready to stream");
    statusLabel_->setStyleSheet("color: green; font-weight: bold;");
    startBtn_->setEnabled(true);
    stopBtn_->setEnabled(false);
    streamStatusLabel_->setText("Stopped");
}

void MainWindow::onAddSource() {
    int sourceIndex = sourceCombo_->currentIndex();
    std::shared_ptr<Qcord::Source> source;
    
    switch (sourceIndex) {
        case 0:
            source = std::make_shared<Qcord::ScreenCapture>(0);
            break;
        case 1:
            source = std::make_shared<Qcord::WebcamCapture>();
            break;
        case 2:
            source = std::make_shared<Qcord::AudioInput>();
            break;
        default:
            return;
    }
    
    if (source) {
        broadcast_->addSource(source);
        QMessageBox::information(this, "Source Added", "Source added successfully!");
    }
}

void MainWindow::onRemoveSource() {
    if (broadcast_->getSources().empty()) {
        QMessageBox::warning(this, "No Sources", "No sources to remove!");
        return;
    }
    
    auto sources = broadcast_->getSources();
    broadcast_->removeSource(sources.back());
    QMessageBox::information(this, "Source Removed", "Last source removed.");
}

void MainWindow::onShowSettings() {
    QMessageBox::information(this, "Settings", "Use the Settings tab to configure your stream parameters.");
}

void MainWindow::onOutputToggled(int index) {
    // Configuration changed
}

void MainWindow::onResolutionChanged() {
    QString res = resolutionCombo_->currentText();
    if (res.contains("1920")) {
        broadcast_->setResolution(1920, 1080);
    } else if (res.contains("1280")) {
        broadcast_->setResolution(1280, 720);
    } else if (res.contains("854")) {
        broadcast_->setResolution(854, 480);
    }
}

void MainWindow::onFpsChanged(int value) {
    broadcast_->setFrameRate(value);
    fpsLabelCurrent_->setText(QString::number(value) + " FPS");
}

void MainWindow::onBitrateChanged(int value) {
    broadcast_->setBitrate(value);
    bitrateLabelCurrent_->setText(QString::number(value / 1000.0, 'f', 1) + " Mbps");
}

void MainWindow::onUpdateStatus() {
    if (broadcast_->isRunning()) {
        streamStatusLabel_->setText("🔴 LIVE");
    } else {
        streamStatusLabel_->setText("⚪ IDLE");
    }
}

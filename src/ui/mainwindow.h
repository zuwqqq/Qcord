#pragma once

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>
#include <memory>

namespace Qcord {
    class Broadcast;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onStartStreaming();
    void onStopStreaming();
    void onAddSource();
    void onRemoveSource();
    void onShowSettings();
    void onOutputToggled(int index);
    void onResolutionChanged();
    void onFpsChanged(int value);
    void onBitrateChanged(int value);
    void onUpdateStatus();

private:
    void setupUI();
    void createMenuBar();
    void createControlPanel();
    void createSourceList();
    void createOutputPanel();
    void createSettings();
    void createStatusBar();
    void updateOutputStatus();

    // UI Components
    QLabel *statusLabel_;
    QPushButton *startBtn_;
    QPushButton *stopBtn_;
    QPushButton *addSourceBtn_;
    QPushButton *removeSourceBtn_;
    QPushButton *settingsBtn_;
    
    // Source list
    QComboBox *sourceCombo_;
    
    // Output checkboxes
    QCheckBox *twitchCheckbox_;
    QCheckBox *youtubeCheckbox_;
    QCheckBox *telegramCheckbox_;
    QCheckBox *recordCheckbox_;
    
    // Settings
    QComboBox *resolutionCombo_;
    QSpinBox *fpsSpinBox_;
    QSpinBox *bitrateSpinBox_;
    
    // Status indicators
    QLabel *streamStatusLabel_;
    QLabel *bitrateLabelCurrent_;
    QLabel *fpsLabelCurrent_;
    
    std::shared_ptr<Qcord::Broadcast> broadcast_;
};

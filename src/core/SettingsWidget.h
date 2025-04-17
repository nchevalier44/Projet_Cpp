#ifndef PROJET_CPP_SETTINGSWIDGET_H
#define PROJET_CPP_SETTINGSWIDGET_H

#include "WindowSizeComboBox.h"

class MainWindow;

class VolumeSlider : public QSlider{
    Q_OBJECT

public:
    VolumeSlider(Qt::Orientation orientation, QWidget* parent=nullptr);

private:
    double ratioHandle = 1;
    double ratioBar = 1;
    QPixmap* barPixmap = nullptr;
    QPixmap* handlePixmap = nullptr;

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    };

class SettingsWidget : public QWidget {
    Q_OBJECT
public:
    SettingsWidget(MainWindow* mainWindow, QWidget* parent=nullptr);

private:
    VolumeSlider* volumeMusicSlider = nullptr;
    VolumeSlider* volumeSFXSlider = nullptr;
    WindowSizeComboBox* windowSizeComboBox = nullptr;
    float ratioSettingsPixmap = 1;
    QPixmap* settingsBackgroundPixmap = nullptr;

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    };

#endif //PROJET_CPP_SETTINGSWIDGET_H

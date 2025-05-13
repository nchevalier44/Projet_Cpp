#ifndef PROJET_CPP_SETTINGSWIDGET_H
#define PROJET_CPP_SETTINGSWIDGET_H

#include "WindowSizeComboBox.h"
#include "VolumeSlider.h"

class MainWindow; //Forward declaration


class SettingsWidget : public QWidget {
    Q_OBJECT
public:
    //Constructor and destructor
    SettingsWidget(MainWindow* mainWindow, QWidget* parent=nullptr);
    virtual ~SettingsWidget();

private:
    float ratioSettingsPixmap = 1;

    VolumeSlider* volumeMusicSlider = nullptr;
    VolumeSlider* volumeSFXSlider = nullptr;
    WindowSizeComboBox* windowSizeComboBox = nullptr;
    QPixmap* settingsBackgroundPixmap = nullptr;

protected:
    //Redifined functions
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void mousePressEvent(QMouseEvent *event) override;


    };

#endif //PROJET_CPP_SETTINGSWIDGET_H

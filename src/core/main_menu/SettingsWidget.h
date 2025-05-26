#ifndef PROJET_CPP_SETTINGSWIDGET_H
#define PROJET_CPP_SETTINGSWIDGET_H

#include <QVBoxLayout>
#include <QLabel>
#include "WindowSizeComboBox.h"
#include "VolumeSlider.h"
#include "MainMenuButton.h"

class MainWindow; //Forward declaration


class SettingsWidget : public QWidget {
    Q_OBJECT
public:
    //Constructor and destructor
    SettingsWidget(MainWindow* mainWindow, QWidget* parent=nullptr);
    virtual ~SettingsWidget();

    //Getters
    VolumeSlider* getVolumeSFXSlider() { return volumeSFXSlider; }
    VolumeSlider* getVolumeMusicSlider() { return volumeMusicSlider; }
    WindowSizeComboBox* getWindowSizeComboBox() { return windowSizeComboBox; }

    //Function that resize all items in settings widget in function of the width and height given
    void resizeItems(int width, int height);

private:
    float ratioSettingsPixmap = 1;
    QPixmap* settingsBackgroundPixmap = nullptr;

    //Elements in settings widget
    VolumeSlider* volumeMusicSlider = nullptr;
    VolumeSlider* volumeSFXSlider = nullptr;
    WindowSizeComboBox* windowSizeComboBox = nullptr;
    MainMenuButton* closeButton = nullptr;

    QFont textFont;
    QFont titleFont;
    QLabel* title = nullptr;
    QLabel* volumeMusicLabel = nullptr;
    QLabel* volumeMusicValueLabel = nullptr;
    QLabel* volumeSFXLabel = nullptr;
    QLabel* volumeSFXValueLabel = nullptr;
    QLabel* windowSizeLabel = nullptr;

    QVBoxLayout* containerLayout = nullptr;
    QHBoxLayout* volumeMusicLayout = nullptr;
    QHBoxLayout* volumeSFXLayout = nullptr;
    QHBoxLayout* windowSizeLayout = nullptr;

protected:
    //Redifined functions
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void mousePressEvent(QMouseEvent *event) override;


    };

#endif //PROJET_CPP_SETTINGSWIDGET_H

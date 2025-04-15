#ifndef PROJET_CPP_STARTMENUSCENE_H
#define PROJET_CPP_STARTMENUSCENE_H

#include <QGraphicsScene>
#include <QPushButton>
#include <QWidget>
#include <QVBoxLayout>
#include <QPixmap>
#include <QLabel>
#include <QPainter>
#include <QApplication>
#include <QFontDatabase>
#include <QGraphicsProxyWidget>
#include <iostream>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QSoundEffect>
#include <QSlider>
#include <QComboBox>

#include "MainMenuButton.h"

class VolumeSlider : public QSlider{
Q_OBJECT
public:
    VolumeSlider(Qt::Orientation orientation, QWidget* parent=nullptr) : QSlider(orientation, parent){}
    virtual ~VolumeSlider() {};

/*protected:
    void paintEvent(QPaintEvent* event) override {
        QSlider::paintEvent(event);  // Dessiner la partie par défaut du slider

        QPainter painter(this);
        QStyleOptionSlider opt;
        initStyleOption(&opt);

        // Dessiner la barre du slider avec une image de fond
        QPixmap grooveImage("../assets/images/menu/slider_bar.png");
        grooveImage = grooveImage.scaled(size(), Qt::KeepAspectRatio);  // Ajuster l'image à la taille du slider
        painter.drawPixmap(0, height() / 2 - grooveImage.height() / 2, grooveImage);

        // Dessiner la poignée du slider avec une image
        QPixmap handleImage("../assets/images/menu/slider_handle.png");
        int handlePos = opt.rect.x() + opt.sliderPosition - handleImage.width() / 2;
        painter.drawPixmap(handlePos, height() / 2 - handleImage.height() / 2, handleImage);
    }*/
};

class SettingsWidget : public QWidget {
    Q_OBJECT
public:
    SettingsWidget(int* volume, QWidget* parent=nullptr);
    virtual ~SettingsWidget() {};

    void resetValues();

public slots:
    void cancelSettings();
    void saveSettings();

private:
    VolumeSlider* volumeSlider = nullptr;
    QComboBox* windowSizeDropdown = nullptr;
    int* volumePercentage = nullptr;
};

class StartMenuScene : public QGraphicsScene{
    Q_OBJECT

    public:
        //Getters
        QPixmap* getBackground() const { return background; }
        QMediaPlayer* getAudioPlayer() const { return audioPlayer; }

        void drawBackground(QPainter* painter, const QRectF& rect);

        StartMenuScene(int* volume, QObject* parent = nullptr);
        virtual ~StartMenuScene() {};

    private:
        QPixmap* background = nullptr;
        QWidget* buttonsContainer = nullptr;
        QMediaPlayer* audioPlayer = nullptr;
        QSoundEffect* sound = nullptr;
        SettingsWidget* settingsWidget = nullptr;
        int* volumePercentage = nullptr;

    signals:
        void startGameRequested();

};


#endif //PROJET_CPP_STARTMENUSCENE_H

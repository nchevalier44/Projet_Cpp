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
#include "SettingsWidget.h"

class MainWindow;

class StartMenuScene : public QGraphicsScene{
    Q_OBJECT

    public:
        //Getters
        QPixmap* getBackground() const { return background; }
        QMediaPlayer* getAudioPlayer() const { return audioPlayer; }

        void drawBackground(QPainter* painter, const QRectF& rect);

        StartMenuScene(MainWindow* mainWindow, QObject* parent = nullptr);

    private:
        QPixmap* background = nullptr;
        QWidget* buttonsContainer = nullptr;
        QMediaPlayer* audioPlayer = nullptr;
        QSoundEffect* sound = nullptr;
        SettingsWidget* settingsWidget = nullptr;

    signals:
        void startGameRequested();

};


#endif //PROJET_CPP_STARTMENUSCENE_H

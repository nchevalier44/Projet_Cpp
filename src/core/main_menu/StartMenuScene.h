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
#include <QTimer>
#include <QStyleFactory>
#include <QGraphicsDropShadowEffect>

#include "MainMenuButton.h"
#include "SettingsWidget.h"
#include "ScoreboardWidget.h"
#include "../AudioManager.h"

class MainWindow;

class StartMenuScene : public QGraphicsScene{
    Q_OBJECT

    public:
        //Getters
        QMediaPlayer* getAudioPlayer() const { return audioPlayer; }

        void createSettingsWidget(MainWindow* mainWindow);
        void createScoreboardWidget(MainWindow* mainWindow);
        void createButtons(MainWindow* mainWindow);
        void createScores(MainWindow* mainWindow);

        void drawBackground(QPainter* painter, const QRectF& rect);

        StartMenuScene(MainWindow* mainWindow, QObject* parent = nullptr);
        virtual ~StartMenuScene();

    private:
        QPixmap* background = nullptr;
        QWidget* buttonsContainer = nullptr;

        QMediaPlayer* audioPlayer = nullptr;
        QSoundEffect* sound = nullptr;
        SettingsWidget* settingsWidget = nullptr;
        ScoreboardWidget* scoreboardWidget = nullptr;
        QGraphicsProxyWidget* settingsProxyWidget = nullptr;
        QGraphicsProxyWidget* scoreboardProxyWidget = nullptr;
        QLabel* titleLabel = nullptr;


    signals:
        void startGameRequested();

};


#endif //PROJET_CPP_STARTMENUSCENE_H

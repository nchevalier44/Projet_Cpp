#ifndef PROJET_CPP_MAINWINDOW_H
#define PROJET_CPP_MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QMessageBox>
#include <QMainWindow>

#include "../constants.h"
#include "GameScene.h"
#include "main_menu/StartMenuScene.h"
#include "AudioManager.h"
#include "ScoreManager.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    GameScene* gameScene = nullptr;
    StartMenuScene* startMenuScene = nullptr;
    MainView* mainView = nullptr;
    HUD* hud = nullptr;
    AudioManager* audioManager = nullptr;
    ScoreManager* scoreManager = nullptr;
    float backgroundRatio = 1;

public:
    void resizeEvent(QResizeEvent* event);

    AudioManager* getAudioManager() const { return audioManager; }
    float getBackgroundRatio() const { return backgroundRatio; }
    ScoreManager* getScoreManager() const { return scoreManager; }
    GameScene* getGameSene() { return gameScene; }
    MainView* getMainView() { return mainView; }

    void setBackgroundRatio(float newRatio) { backgroundRatio = newRatio; }

    void resetGame();

    MainWindow(QWidget* parent = nullptr);
    virtual ~MainWindow();

public slots:
    void startGame();
    void goToStartMenu();
    void restartGame();
};

#endif //PROJET_CPP_MAINWINDOW_H

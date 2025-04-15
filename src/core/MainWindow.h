#ifndef PROJET_CPP_MAINWINDOW_H
#define PROJET_CPP_MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QMessageBox>
#include <QMainWindow>

#include "GameScene.h"
#include "StartMenuScene.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

private :
    GameScene* gameScene = nullptr;
    StartMenuScene* startMenuScene = nullptr;
    MainView* mainView = nullptr;
    HUD* hud = nullptr;
    int volumePercentage;
    float backgroundRatio = 0.5;

public:
    void resizeEvent(QResizeEvent* event);

    void resetGame();

    MainWindow(QWidget* parent = nullptr);
    virtual ~MainWindow();

public slots:
    void startGame();
    void goToStartMenu();
    void restartGame();
};

#endif //PROJET_CPP_MAINWINDOW_H

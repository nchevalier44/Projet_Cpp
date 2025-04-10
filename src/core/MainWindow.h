#ifndef PROJET_CPP_MAINWINDOW_H
#define PROJET_CPP_MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QMessageBox>

#include "GameScene.h"
#include "StartMenuScene.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

private :
    GameScene* gameScene;
    StartMenuScene* startMenuScene;
    MainView* mainView;


public:
    MainWindow(QWidget* parent = nullptr);
    virtual ~MainWindow();

public slots:
    void startGame();
};

#endif //PROJET_CPP_MAINWINDOW_H

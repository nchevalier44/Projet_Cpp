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
    GameScene* gameScene;
    StartMenuScene* startMenuScene;
    MainView* mainView;
    float windowRatio = 0.5;

public:
    //Getters
    double getWindowRatio() const { return windowRatio; }

    //Setters
    void setWindowRatio(double newRatio) { windowRatio = newRatio; }

    void resizeEvent(QResizeEvent* event);

    MainWindow(QWidget* parent = nullptr);
    virtual ~MainWindow();

public slots:
    void startGame();
};

#endif //PROJET_CPP_MAINWINDOW_H

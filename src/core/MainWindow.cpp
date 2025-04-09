#include "MainWindow.h"
#include "MainView.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    startMenuScene = new StartMenuScene();
    mainView = new MainView();
    mainView->setScene(startMenuScene);

    connect(startMenuScene, &StartMenuScene::startGameRequested, this, &MainWindow::startGame);

    this->setCentralWidget(mainView);
    this->setWindowTitle("C++ Project");
}

MainWindow::~MainWindow(){

}

void MainWindow::startGame(){
    gameScene = new GameScene();
    mainView->setScene(gameScene);
}
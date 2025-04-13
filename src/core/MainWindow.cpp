#include "MainWindow.h"
#include "MainView.h"
#include "HUD.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    startMenuScene = new StartMenuScene();
    mainView = new MainView();
    mainView->setScene(startMenuScene);

    connect(startMenuScene, &StartMenuScene::startGameRequested, this, &MainWindow::startGame); //When the player start from the principal menu
    connect(mainView, &MainView::startGameRequested, this, &MainWindow::restartGame); //When the player die and he clicked on restart
    connect(mainView, &MainView::goToStartMenuRequested, this, &MainWindow::goToStartMenu); //When the player die and he want to go back to the principal menu

    this->setCentralWidget(mainView);
    this->setWindowTitle("C++ Project");

    QPixmap* background = this->startMenuScene->getBackground();
    this->backgroundRatio = float(background->width()) / float(background->height());
    this->resize(background->width()*0.75, background->height()*0.75);

}

MainWindow::~MainWindow(){

}

void MainWindow::restartGame(){
    resetGame();
    startGame();

}

void MainWindow::goToStartMenu() {
    resetGame();
    mainView->setScene(startMenuScene);
    mainView->setFitView(true);
}

void MainWindow::startGame(){
    //Set the scene to the game scene
    gameScene = new GameScene(mainView);
    gameScene->setView(mainView);
    mainView->setScene(gameScene);
    mainView->setFitView(false);
    mainView->scale(1.5, 1.5);

    //Setting up the HUD
    hud = new HUD(gameScene->getCharacter()->getMaxHp(), this->mainView);
    hud->move(10, 10);
    hud->raise();
    gameScene->getCharacter()->setHUD(hud);
}

void MainWindow::resetGame(){
    mainView->deleteDeathScreen();
    mainView->scale(0.66666666, 0.66666666); // 0.666666 to reset the view to 1:1 which was changed by scale(1.5, 1.5) in startGame
    if(gameScene != nullptr){
        delete gameScene;
        gameScene = nullptr;
    }
    if(hud != nullptr){
        delete hud;
        hud = nullptr;
    }
}


void MainWindow::resizeEvent(QResizeEvent* event) {
    QMainWindow::resizeEvent(event);

    QSize currentSize = event->size();

    int newWidth = currentSize.width();
    int newHeight = newWidth / backgroundRatio;

    if (newHeight > currentSize.height()) {
        newHeight = currentSize.height();
        newWidth = newHeight * backgroundRatio;
    }

    this->resize(newWidth, newHeight);

}
#include "MainWindow.h"
#include "MainView.h"
#include "HUD.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    audioManager = new AudioManager();
    scoreManager = new ScoreManager();
    scoreManager->loadScores();

    startMenuScene = new StartMenuScene(this);
    mainView = new MainView(scoreManager, this);
    mainView->setScene(startMenuScene);

    connect(startMenuScene, &StartMenuScene::startGameRequested, this, &MainWindow::startGame); //When the player start from the principal menu
    connect(mainView, &MainView::startGameRequested, this, &MainWindow::restartGame); //When the player die and he clicked on restart
    connect(mainView, &MainView::goToStartMenuRequested, this, &MainWindow::goToStartMenu); //When the player die and he want to go back to the principal menu

    this->setCentralWidget(mainView);
    this->setWindowTitle("C++ Project");
    this->setFixedSize(backgroundRatio * DEFAULT_WINDOW_HEIGHT, DEFAULT_WINDOW_HEIGHT);
}


MainWindow::~MainWindow(){
    delete audioManager;
    audioManager = nullptr;
    delete scoreManager;
    scoreManager = nullptr;
}

void MainWindow::restartGame(){
    resetGame();
    startGame();

}

void MainWindow::goToStartMenu() {
    resetGame();
    mainView->setScene(startMenuScene);
    mainView->setFitView(true);
    startMenuScene->getAudioPlayer()->play();
    scoreManager->loadScores();
    startMenuScene->createScores(this);
}

void MainWindow::startGame(){
    //Stop music
    startMenuScene->getAudioPlayer()->stop();

    //Set the scene to the game scene
    gameScene = new GameScene(mainView, scoreManager, this);
    gameScene->setView(mainView);
    mainView->setScene(gameScene);
    mainView->setFitView(false);
    mainView->scale(4, 4);
    QPointF windowSize(this->width(), this->height());

    //Setting up the HUD
    hud = new HUD(gameScene->getCharacter()->getMaxHp(),windowSize, this->mainView);
    hud->move(0, 0);
    hud->raise();
    gameScene->setHUD(hud);
    gameScene->getCharacter()->setHUD(hud);

    scoreManager->getElapsedTimer()->start();
}

void MainWindow::resetGame(){
    mainView->deleteDeathScreen();
    mainView->scale(0.25, 0.25); // 0.25 to reset the view to 1:1 which was changed by scale(4, 4) in startGame
    if(gameScene != nullptr){
        delete gameScene;
        gameScene = nullptr;
    }
    if(hud != nullptr){
        delete hud;
        hud = nullptr;
    }
    delete scoreManager->getActualScore();
    scoreManager->setActualScore(new Score());

}


void MainWindow::resizeEvent(QResizeEvent* event) {
    QMainWindow::resizeEvent(event);

    QSize newSize = event->size();

    int newWidth = newSize.width();
    int newHeight = newWidth / backgroundRatio;

    if (newHeight != newSize.height()) {
        newHeight = newSize.height();
        newWidth = newHeight * backgroundRatio;
    }

    this->resize(newWidth, newHeight);

}
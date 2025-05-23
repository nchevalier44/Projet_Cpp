#include "MainWindow.h"
#include "MainView.h"
#include "HUD.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    audioManager = new AudioManager();
    scoreManager = new ScoreManager();
    scoreManager->loadScores();


    startMenuScene = new StartMenuScene(this);
    mainView = new MainView(this, scoreManager, this);
    mainView->setScene(startMenuScene);

    connect(startMenuScene, &StartMenuScene::startGameRequested, this, &MainWindow::startGame); //When the player start from the principal menu
    connect(mainView, &MainView::startGameRequested, this, &MainWindow::restartGame); //When the player die and he clicked on restart
    connect(mainView, &MainView::goToStartMenuRequested, this, &MainWindow::goToStartMenu); //When the player die and he want to go back to the principal menu

    this->setCentralWidget(mainView);
    this->setWindowTitle("C++ Project");
    this->setFixedSize(backgroundRatio * DEFAULT_WINDOW_HEIGHT, DEFAULT_WINDOW_HEIGHT);
    startMenuScene->getSettingsWidget()->getWindowSizeComboBox()->setCurrentText(QString::number(backgroundRatio * DEFAULT_WINDOW_HEIGHT) + "x" + QString::number(DEFAULT_WINDOW_HEIGHT));

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

    scoreManager->loadScores();

    startMenuScene->getAudioPlayer()->play();
    mainView->setScene(startMenuScene);
    mainView->setFitView(true);
    mainView->fitInView(mainView->sceneRect(), Qt::KeepAspectRatio);
    qDebug() << "oui";
}

void MainWindow::startGame(){
    //Stop music
    startMenuScene->getAudioPlayer()->stop();

    //Set the scene to the game scene
    gameScene = new GameScene(mainView, scoreManager, this);
    gameScene->setView(mainView);
    mainView->setScene(gameScene);
    mainView->setFitView(false);

    mainView->resetTransform();
    float s = 1.35 * float(this->height()) / DEFAULT_WINDOW_HEIGHT;
    mainView->scale(s, s);

    //Setting up the HUD
    hud = new HUD(gameScene->getCharacter()->getMaxHp(),this, this->mainView);
    hud->move(0, 0);
    hud->raise();
    gameScene->setHUD(hud);
    gameScene->getCharacter()->setHUD(hud);

    scoreManager->getElapsedTimer()->start();
}

void MainWindow::resetGame(){
    mainView->deleteDeathScreen();
    mainView->resetTransform();
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

        if(newSize == event->oldSize()) return;

        int newWidth = newSize.width();
        int newHeight = newWidth / backgroundRatio;

        if (newHeight != newSize.height()) {
            newHeight = newSize.height();
            newWidth = newHeight * backgroundRatio;
        }

        QResizeEvent re(QSize(newWidth, newHeight), QSize(this->width(), this->height()));
        this->setFixedSize(newWidth, newHeight);
        mainView->resizeEvent(&re);

}
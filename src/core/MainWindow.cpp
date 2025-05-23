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

    QTimer* windowSizeTimer = new QTimer(this);
    windowSizeTimer->start(500);
    connect(windowSizeTimer, &QTimer::timeout, this, [=](){
        this->checkRatio(this->size(), true);
        qDebug() << "check";
    });
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
}

void MainWindow::startGame(){
    //Stop music
    startMenuScene->getAudioPlayer()->stop();

    //Set the scene to the game scene
    gameScene = new GameScene(this,mainView, scoreManager, this);
    gameScene->setView(mainView);
    mainView->setScene(gameScene);
    mainView->setFitView(false);

    mainView->resetTransform();
    float s = 1.35 * float(this->height()) / DEFAULT_WINDOW_HEIGHT;
    mainView->scale(s, s);

    //Setting up the HUD
    QPointF windowSize(this->width(), this->height());
    hud = new HUD(gameScene->getCharacter()->getMaxHp(),windowSize, this->mainView);
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

        if (newSize == event->oldSize()) return;
        newSize = checkRatio(newSize);
        if (newSize == event->oldSize()) return;

        QResizeEvent eventResize(newSize, QSize(this->width(), this->height()));
        this->resize(newSize);
        mainView->resizeEvent(&eventResize);
}

QSize MainWindow::checkRatio(QSize size, bool check){
    int newWidth = size.width();
    int newHeight = newWidth / backgroundRatio;

    if (newHeight != size.height()) {
        newHeight = size.height();
        newWidth = newHeight * backgroundRatio;
    }

    if(check){
        if(size != QSize(newWidth, newHeight) || mainView->size() != this->size()){
            qDebug() << "Size not good" << size << QSize(newWidth, newHeight) << mainView->size();
            this->resize(QSize(newWidth, newHeight));
            mainView->setFixedSize(QSize(newWidth, newHeight));
        }

    }
    return QSize(newWidth, newHeight);


}


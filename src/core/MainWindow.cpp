#include "MainWindow.h"
#include "MainView.h"
#include "HUD.h"

//Constructor
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    //Create audio and score manager
    audioManager = new AudioManager();
    scoreManager = new ScoreManager();
    scoreManager->loadScores();

    //Create StartMenuScene and MainView
    startMenuScene = new StartMenuScene(this);
    mainView = new MainView(this, scoreManager, this);
    mainView->setScene(startMenuScene);


    //Actions
    connect(startMenuScene, &StartMenuScene::startGameRequested, this, &MainWindow::startGame); //When the player start from the principal menu
    connect(mainView, &MainView::startGameRequested, this, &MainWindow::restartGame); //When the player die and he clicked on restart
    connect(mainView, &MainView::goToStartMenuRequested, this, &MainWindow::goToStartMenu); //When the player die, and he wants to go back to the principal menu


    this->setCentralWidget(mainView);
    this->setWindowTitle("C++ Project");
    this->setFixedSize(backgroundRatio * DEFAULT_WINDOW_HEIGHT, DEFAULT_WINDOW_HEIGHT);
    startMenuScene->getSettingsWidget()->getWindowSizeComboBox()->setCurrentText(QString::number(backgroundRatio * DEFAULT_WINDOW_HEIGHT) + "x" + QString::number(DEFAULT_WINDOW_HEIGHT));


    //Check ratio of the window every 500ms and modify it if the ratio is not correct
    //Because sometimes Qt call multiple times resizeEvent, so it doesn't work well sometimes
    QTimer* windowSizeTimer = new QTimer(this);
    windowSizeTimer->start(500);
    connect(windowSizeTimer, &QTimer::timeout, this, [=](){
        this->checkRatio(this->size(), true);
    });
}

//Destructor
MainWindow::~MainWindow(){
    delete audioManager;
    audioManager = nullptr;
    delete scoreManager;
    scoreManager = nullptr;
}

//Restart the game
void MainWindow::restartGame(){
    scoreManager->addScore(*(scoreManager->getActualScore()));
    resetGame();
    startGame();

}

//Go to start menu scene
void MainWindow::goToStartMenu() {

    scoreManager->addScore(*(scoreManager->getActualScore()));
    resetGame();

    scoreManager->loadScores();

    startMenuScene->createButtons(this); //Update scoreboardWidget and settingsWidget
    startMenuScene->getAudioPlayer()->play();

    //Change the actual scene
    mainView->setScene(startMenuScene);
    mainView->setFitView(true);
    mainView->fitInView(mainView->sceneRect(), Qt::KeepAspectRatio);

    startMenuScene->getAudioPlayer()->play(); //Start the music
}

//Start a game
void MainWindow::startGame(){
    //Set the scene to the game scene
    gameScene = new GameScene(audioManager, mainView, scoreManager, this);
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

    //Start the elapsed timer in score manager to get the time played
    scoreManager->getElapsedTimer()->start();

    mainView->setFocusPolicy(Qt::StrongFocus);
    mainView->setFocus();
    mainView->setMouseTracking(true);
}

//Reset game
void MainWindow::resetGame(){
    mainView->deleteDeathScreen();
    mainView->deleteWinScreen();
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

//Redefinition of resizeEvent to keep the right ratio
void MainWindow::resizeEvent(QResizeEvent* event) {
        QMainWindow::resizeEvent(event);
        QSize newSize = event->size();

        if (newSize == event->oldSize()) return;
        newSize = checkRatio(newSize);
        if (newSize == event->oldSize()) return;

        //Call resizeEvent of mainView because it is not called everytime
        QResizeEvent eventResize(newSize, QSize(this->width(), this->height()));
        this->resize(newSize);
        mainView->resizeEvent(&eventResize);
}

//Check the ratio of the window and return the right size adjusted
QSize MainWindow::checkRatio(QSize size, bool check){
    int newWidth = size.width();
    int newHeight = newWidth / backgroundRatio;

    if (newHeight != size.height()) {
        newHeight = size.height();
        newWidth = newHeight * backgroundRatio;
    }

    if(check){
        if(size != QSize(newWidth, newHeight) || mainView->size() != this->size()){
            this->resize(QSize(newWidth, newHeight));
            mainView->setFixedSize(QSize(newWidth, newHeight));
        }

    }
    return QSize(newWidth, newHeight);


}


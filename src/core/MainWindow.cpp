#include "MainWindow.h"
#include "MainView.h"
#include "HUD.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    startMenuScene = new StartMenuScene();
    mainView = new MainView();
    mainView->setScene(startMenuScene);

    connect(startMenuScene, &StartMenuScene::startGameRequested, this, &MainWindow::startGame);

    this->setCentralWidget(mainView);
    this->setWindowTitle("C++ Project");

    QPixmap* background = startMenuScene->getBackground();
    this->windowRatio = float(background->width()) / float(background->height());
    //this->setFixedSize(background->width(), background->height());

}

MainWindow::~MainWindow(){

}

void MainWindow::startGame(){
    //Set the scene to the game scene
    gameScene = new GameScene();
    //Setting up the HUD
    HUD* hud = new HUD(gameScene->getCharacter()->getMaxHp(), this->mainView);
    hud->move(10, 10);
    hud->raise();


    gameScene->setView(mainView);
    mainView->setScene(gameScene);
    mainView->setFitView(false);
    mainView->scale(1.5, 1.5);
}


void MainWindow::resizeEvent(QResizeEvent* event) {
    static bool isResizing = false;

    if (isResizing) {
        return;
    }

    isResizing = true;

    QSize oldSize = event->oldSize();
    QSize newSize = event->size();

    int ideal_width = newSize.width();
    int ideal_height = newSize.height();

    if (oldSize.height() == newSize.height()) {
        ideal_height = newSize.width() / windowRatio;
    } else {
        ideal_width = newSize.height() * windowRatio;
    }
    this->resize(ideal_width, ideal_height);

    isResizing = false;
}
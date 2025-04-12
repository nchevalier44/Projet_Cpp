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
    this->window_ratio = float(background->width()) / float(background->height());
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

#include <QMainWindow>

void MainWindow::resizeEvent(QResizeEvent* event) {
    static bool is_resizing = false;

    if (is_resizing) {
        return;
    }

    is_resizing = true;

    QSize old_size = event->oldSize();
    QSize new_size = event->size();

    int ideal_width = new_size.width();
    int ideal_height = new_size.height();

    if (old_size.height() == new_size.height()) {
        ideal_height = new_size.width() / window_ratio;
    } else {
        ideal_width = new_size.height() * window_ratio;
    }
    this->resize(ideal_width, ideal_height);

    is_resizing = false;
}
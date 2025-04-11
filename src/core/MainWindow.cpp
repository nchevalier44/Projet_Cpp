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

    QPixmap* background = startMenuScene->getBackground();
    //this->window_ratio = background->width() / background->height();
}

MainWindow::~MainWindow(){

}

void MainWindow::startGame(){
    gameScene = new GameScene();
    mainView->setFitView(false);
    gameScene->setView(mainView);
    mainView->setScene(gameScene);
    mainView->scale(1.5, 1.5);
}

/*void MainWindow::resizeEvent(QResizeEvent* event) {
    //Force the window to keep the ratio of the background
    qDebug() << window_ratio;
    QSize new_size = event->size();
    int ideal_width = new_size.height() * window_ratio;
    if (new_size.width() != ideal_width) {
        this->resize(ideal_width, new_size.height());
    }

    QMainWindow::resizeEvent(event);
}*/
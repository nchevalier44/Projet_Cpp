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
    this->windowRatio = float(background->width()) / float(background->height());
    //this->setFixedSize(background->width(), background->height());

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
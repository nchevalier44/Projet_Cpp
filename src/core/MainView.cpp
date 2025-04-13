#include "MainView.h"
#include <QTimer>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QLabel>
#include <QMovie>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFontDatabase>


MainView::MainView(QWidget* parent) : QGraphicsView(parent) {
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

MainView::~MainView() {}

void MainView::deleteDeathScreen(){
    if(deathScreen != nullptr){
        delete deathScreen;
        deathScreen = nullptr;
    }
}

void MainView::resizeEvent (QResizeEvent* event){
    QGraphicsView::resizeEvent(event);

    //We want to have the dimension of the view equal to the scene only in the menu but no in game
    if(fitView) {
        this->fitInView(sceneRect(), Qt::KeepAspectRatio);
    }

    if(deathScreen != nullptr){
        deathScreen->setGeometry(this->rect());
    }

}

void MainView::displayDeathScreen() {
    //Width : contentContainer is 65% of the window : 27% buttonBack, 27% button restart, 11% spacing | 67% title
    //Height : contentContainer is 50% of the window : 19% spacing, 20% title, 7% buttons

    //Add font Jersey10 (pixel art)
    int fontId = QFontDatabase::addApplicationFont("../assets/fonts/Jersey10-Regular.ttf");
    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    qDebug() << this->window()->width() << ", " << this->window()->height();
    QFont buttonFont(fontFamily);
    QFont titleFont(fontFamily);
    buttonFont.setPixelSize(this->window()->height()*0.07);
    titleFont.setPixelSize(this->window()->height()*0.2);


    deathScreen = new QWidget(this);
    deathScreen->setGeometry(this->rect());
    deathScreen->show();

    //Creation of black background
    QWidget* blackBackground = new QWidget(deathScreen);
    blackBackground->setStyleSheet("background-color: black;");
    blackBackground->setGeometry(deathScreen->rect());
    blackBackground->show();

    //Creation of title and buttons
    QLabel* title = new QLabel("Game over !");
    title->setStyleSheet("color: white;");
    title->setAlignment(Qt::AlignCenter);
    title->setFont(titleFont);
    title->adjustSize();
    QPushButton* buttonRestart = new QPushButton("Restart the game");
    buttonRestart->setFont(buttonFont);
    QObject::connect(buttonRestart, &QPushButton::clicked, this, &MainView::startGameRequested);
    QPushButton* buttonBackToMenu = new QPushButton("Back to the menu");
    buttonBackToMenu->setFont(buttonFont);
    QObject::connect(buttonBackToMenu, &QPushButton::clicked, this, &MainView::goToStartMenuRequested);

    //Creation buttons layout
    QHBoxLayout* buttonsLayout = new QHBoxLayout();
    //buttonsLayout->setSpacing(150*ratioWidth);
    buttonsLayout->addWidget(buttonRestart);
    buttonsLayout->addWidget(buttonBackToMenu);

    //Creation container layout
    QVBoxLayout* containerLayout = new QVBoxLayout();
    containerLayout->setSpacing(this->window()->height()*0.19);
    containerLayout->addWidget(title, Qt::AlignHCenter);
    containerLayout->addLayout(buttonsLayout, Qt::AlignCenter);

    //Creation container widget
    QWidget* contentContainer = new QWidget(deathScreen);
    contentContainer->setLayout(containerLayout);
    contentContainer->show();
    contentContainer->adjustSize();
    contentContainer->move((deathScreen->width() - contentContainer->width()) / 2,
                           (deathScreen->height() - contentContainer->height()) / 2);


    //Fade of black background
    QGraphicsOpacityEffect* fadeBackgroundEffect = new QGraphicsOpacityEffect(blackBackground);
    blackBackground->setGraphicsEffect(fadeBackgroundEffect);
    QPropertyAnimation *animationBackground = new QPropertyAnimation(fadeBackgroundEffect, "opacity");
    animationBackground->setDuration(500);
    animationBackground->setStartValue(0);
    animationBackground->setEndValue(0.6);
    animationBackground->start(QAbstractAnimation::DeleteWhenStopped);

    //Fade of other content
    QGraphicsOpacityEffect* fadeContentEffect = new QGraphicsOpacityEffect(contentContainer);
    contentContainer->setGraphicsEffect(fadeContentEffect);
    QPropertyAnimation *animationContent = new QPropertyAnimation(fadeContentEffect, "opacity");
    animationContent->setDuration(500);
    animationContent->setStartValue(0);
    animationContent->setEndValue(1);
    animationContent->start(QAbstractAnimation::DeleteWhenStopped);

    contentContainer->setStyleSheet("background-color: blue;");
    buttonRestart->setStyleSheet("background-color: red;");
    buttonBackToMenu->setStyleSheet("background-color: green;");
    title->setStyleSheet("background-color: yellow;");



    qDebug() << "contentContainer : " << contentContainer->width() << contentContainer->height();
    qDebug() << "buttonBackToMenu : " << buttonBackToMenu->width() << buttonBackToMenu->height();
    qDebug() << "buttonsRestart : " << buttonRestart->width() << buttonRestart->height();
    qDebug() << "QLabel : " << title->width() << title->height();

}

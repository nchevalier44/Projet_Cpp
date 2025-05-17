#include "MainView.h"
#include "FontManager.h"
#include <QTimer>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QLabel>
#include <QMovie>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFontDatabase>

#include <QResizeEvent>

MainView::MainView(ScoreManager* scoreManager, QWidget* parent) : scoreManager(scoreManager), QGraphicsView(parent) {
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
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

    qDebug() << "resize view";

    //We want to have the dimension of the view equal to the scene only in the menu but no in game
    if(fitView && event->oldSize() != event->size()) {
        this->fitInView(sceneRect(), Qt::KeepAspectRatio);
    }

    if(deathScreen != nullptr){
        deathScreen->setGeometry(this->rect());
    }
}

void MainView::displayDeathScreen() {

    //Add font Jersey10 (pixel art)
    QFont buttonFont(FontManager::fontFamily);
    QFont titleFont(FontManager::fontFamily);
    QFont scoreFont(FontManager::fontFamily);
    buttonFont.setPixelSize(this->window()->height()*0.07); //setPixelSize set the pixel number of the height of a line (letter + margins)
    titleFont.setPixelSize(this->window()->height()*0.2);
    scoreFont.setPixelSize(this->window()->height()*0.05);



    deathScreen = new QWidget(this);
    deathScreen->setGeometry(this->rect());
    deathScreen->show();

    //Creation of black background
    QWidget* blackBackground = new QWidget(deathScreen);
    blackBackground->setStyleSheet("background-color: black;");
    blackBackground->setGeometry(deathScreen->rect());
    blackBackground->show();

    QWidget* contentContainer = new QWidget(deathScreen);

    //Creation of title and buttons
    QLabel* title = new QLabel("Game over !", contentContainer);
    title->setStyleSheet("color: white;");
    title->setAlignment(Qt::AlignCenter);
    title->setFont(titleFont);
    title->adjustSize();

    QPushButton* buttonRestart = new QPushButton("Restart the game", contentContainer);
    buttonRestart->setFont(buttonFont);
    QObject::connect(buttonRestart, &QPushButton::clicked, this, &MainView::startGameRequested);
    QPushButton* buttonBackToMenu = new QPushButton("Back to the menu", contentContainer);
    buttonBackToMenu->setFont(buttonFont);
    QObject::connect(buttonBackToMenu, &QPushButton::clicked, this, &MainView::goToStartMenuRequested);

    int score = scoreManager->getActualScore()->getScore();
    int seconds = scoreManager->getActualScore()->getTimePlayed();
    QString timePlayed = QString::number(seconds / 60) + "m" + QString::number(seconds % 60) + "s";

    QLabel* scoreLabel = new QLabel(contentContainer);
    scoreLabel->setText(QString::number(score) + " pts - " + timePlayed);
    scoreLabel->setStyleSheet("color: white;");
    scoreLabel->setAlignment(Qt::AlignCenter);
    scoreLabel->setFont(scoreFont);
    scoreLabel->adjustSize();



    //Creation buttons layout
    QHBoxLayout* buttonsLayout = new QHBoxLayout();
    buttonsLayout->addWidget(buttonRestart);
    buttonsLayout->addWidget(buttonBackToMenu);

    //Creation container layout
    QVBoxLayout* containerLayout = new QVBoxLayout(contentContainer);
    containerLayout->setSpacing(this->window()->height()*0.1);
    containerLayout->addWidget(title, Qt::AlignHCenter);
    containerLayout->addWidget(scoreLabel, Qt::AlignHCenter);
    containerLayout->addLayout(buttonsLayout, Qt::AlignCenter);

    //Creation container widget
    contentContainer->show();
    contentContainer->adjustSize();
    contentContainer->move((deathScreen->width() - contentContainer->width()) / 2,
                           (deathScreen->height() - contentContainer->height()) / 2);


    //Fade of black background
    QGraphicsOpacityEffect* fadeBackgroundEffect = new QGraphicsOpacityEffect(blackBackground);
    blackBackground->setGraphicsEffect(fadeBackgroundEffect);
    QPropertyAnimation* animationBackground = new QPropertyAnimation(fadeBackgroundEffect, "opacity", blackBackground);
    animationBackground->setDuration(500);
    animationBackground->setStartValue(0);
    animationBackground->setEndValue(0.6);
    animationBackground->start(QAbstractAnimation::DeleteWhenStopped);

    //Fade of other content
    QGraphicsOpacityEffect* fadeContentEffect = new QGraphicsOpacityEffect(contentContainer);
    contentContainer->setGraphicsEffect(fadeContentEffect);
    QPropertyAnimation* animationContent = new QPropertyAnimation(fadeContentEffect, "opacity", contentContainer);
    animationContent->setDuration(500);
    animationContent->setStartValue(0);
    animationContent->setEndValue(1);
    animationContent->start(QAbstractAnimation::DeleteWhenStopped);
}

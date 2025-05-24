#include "MainView.h"
#include "FontManager.h"
#include "main_menu/MainMenuButton.h"
#include "main_menu/SettingsWidget.h"
#include <QTimer>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFontDatabase>

#include <QResizeEvent>
#include "MainWindow.h"

MainView::MainView(MainWindow* mainWindow, ScoreManager* scoreManager, QWidget* parent) : mainWindow(mainWindow), scoreManager(scoreManager), QGraphicsView(parent) {
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    lastSize.setWidth(-1);
    lastSize.setHeight(-1);
    QTimer::singleShot(50, this, [this]() {
        this->fitInView(sceneRect(), Qt::KeepAspectRatio);
    });
}

MainView::~MainView() {
}

void MainView::deleteDeathScreen(){
    if(deathScreen != nullptr){
        delete deathScreen;
        deathScreen = nullptr;
    }
}

void MainView::deleteWinScreen(){
    if(winScreen && winScreen != nullptr) {
        delete winScreen;
        winScreen = nullptr;
    }
}

void MainView::resizeEvent (QResizeEvent* event){
    int deltaW = std::abs(lastSize.width() - event->size().width());
    int deltaH = std::abs(lastSize.height() - event->size().height());
    if (deltaW < 5 && deltaH < 5) return;

    QGraphicsView::resizeEvent(event);

    //We want to have the dimension of the view equal to the scene only in the menu but no in game
    if(event->oldSize() != event->size()){
        if(fitView) {
            this->fitInView(sceneRect(), Qt::KeepAspectRatio);
        } else{
            //Change size of mainView
            this->setFixedSize(event->size());

            //Scale
            float sy = 1.35 * float(event->size().height()) / float(DEFAULT_WINDOW_HEIGHT);
            this->resetTransform();
            this->scale(sy, sy);

            //Center on player
            QTimer::singleShot(0, this, [=]() {
                Player* player = mainWindow->getGameSene()->getCharacter();
                if(player) this->centerOn(player->sceneBoundingRect().center());
            });

            //Update size of pauseScreen elements
            if(pauseScreen){
                updatePauseScreenSize(event->size());
            }

            HUD* hud = mainWindow->getHUD();
            if(hud){
                QPointF windowSize(this->width(), this->height());
                Player* p = mainWindow->getGameSene()->getCharacter();
                if(p) {
                    hud->updateHUD(p->getHp(), hud->getHPWidget()->getMaxLife(), windowSize);
                }
            }
        }
        lastSize = event->size();
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

void MainView::displayWinScreen() {
    //Add font Jersey10 (pixel art)
    QFont buttonFont(FontManager::fontFamily);
    QFont titleFont(FontManager::fontFamily);
    QFont secondTitleFont(FontManager::fontFamily);
    QFont scoreFont(FontManager::fontFamily);
    buttonFont.setPixelSize(this->window()->height()*0.07); //setPixelSize set the pixel number of the height of a line (letter + margins)
    titleFont.setPixelSize(this->window()->height()*0.15);
    secondTitleFont.setPixelSize(this->window()->height()*0.05);
    scoreFont.setPixelSize(this->window()->height()*0.05);



    deathScreen = new QWidget(this);
    deathScreen->setGeometry(this->rect());
    deathScreen->show();

    //Creation of black background
    QWidget* blackBackground = new QWidget(deathScreen);
    blackBackground->setStyleSheet("background-color: green;");
    blackBackground->setGeometry(deathScreen->rect());
    blackBackground->show();

    QWidget* contentContainer = new QWidget(deathScreen);

    //Creation of title and buttons
    QLabel* title = new QLabel("Thank you for playing !", contentContainer);
    title->setStyleSheet("color: white;");
    title->setAlignment(Qt::AlignCenter);
    title->setFont(titleFont);
    title->adjustSize();

    QLabel* secondTitle = new QLabel("You saved the Tree and restored peace in the valley", contentContainer);
    secondTitle->setStyleSheet("color: white;");
    secondTitle->setAlignment(Qt::AlignCenter);
    secondTitle->setFont(secondTitleFont);
    secondTitle->adjustSize();

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

void MainView::displayPauseMenu(){

    if(pauseScreen){
        delete pauseScreen;
        pauseScreen = nullptr;
    }
    pauseScreen = new QWidget(this);
    pauseScreen->setGeometry(this->rect());
    pauseScreen->show();

    //Creation of black background
    blackBackground = new QWidget(pauseScreen);
    blackBackground->setStyleSheet("background-color: black;");
    blackBackground->setGeometry(pauseScreen->rect());
    blackBackground->show();


    //Settings widget
    settingsWidget = new SettingsWidget(mainWindow, this);
    settingsWidget->setFixedSize(QSize(this->width() * 0.6, 1)); //1 because the resizeEvent keep the ratio and only work with the width
    settingsWidget->show();
    settingsWidget->move((pauseScreen->width() - settingsWidget->width()) / 2, (pauseScreen->height() - settingsWidget->height()) / 2);
    settingsWidget->hide();
    connect(settingsWidget->getWindowSizeComboBox(), &QComboBox::currentTextChanged, this, [=](const QString &text){
        for (int h : LIST_WINDOW_HEIGHT ){
            int newWidth = mainWindow->getBackgroundRatio() * h;
            QString windowSize = QString::number(newWidth) + "x" + QString::number(h);

            if(text == windowSize){
                this->setFixedSize(width(), height());
            } else if(text == "Custom"){
                mainWindow->setMinimumSize(QSize(0, 0));
                mainWindow->setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
            }
        }
    });

    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect(settingsWidget);
    shadowEffect->setOffset(0, 0);
    shadowEffect->setBlurRadius(50);
    shadowEffect->setColor(Qt::black);
    settingsWidget->setGraphicsEffect(shadowEffect);

    createContentPauseContainer();

    //Fade of black background
    QGraphicsOpacityEffect* fadeBackgroundEffect = new QGraphicsOpacityEffect(blackBackground);
    blackBackground->setGraphicsEffect(fadeBackgroundEffect);
    QPropertyAnimation* animationBackground = new QPropertyAnimation(fadeBackgroundEffect, "opacity", blackBackground);
    animationBackground->setDuration(250);
    animationBackground->setStartValue(0);
    animationBackground->setEndValue(0.3);
    animationBackground->start(QAbstractAnimation::DeleteWhenStopped);
}

void MainView::createContentPauseContainer(){

    if(contentContainer){
        delete contentContainer;
        contentContainer = nullptr;
    }
    contentContainer = new QWidget(pauseScreen);

    //Add font Jersey10 (pixel art)
    QFont buttonFont(FontManager::fontFamily);
    QFont titleFont(FontManager::fontFamily);
    QFont scoreFont(FontManager::fontFamily);
    buttonFont.setPixelSize(this->window()->height()*0.04); //setPixelSize set the pixel number of the height of a line (letter + margins)
    titleFont.setPixelSize(this->window()->height()*0.1);
    scoreFont.setPixelSize(this->window()->height()*0.06);

    //Creation of title and buttons
    QLabel* titlePause = new QLabel("Pause Menu", contentContainer);
    titlePause->setStyleSheet("color: white;");
    titlePause->setAlignment(Qt::AlignCenter);
    titlePause->setFont(titleFont);
    titlePause->adjustSize();

    //Buttons
    MainMenuButton* buttonBackToMenu = new MainMenuButton("Back to the menu", mainWindow->getAudioManager(), contentContainer);
    MainMenuButton* buttonBackToGame = new MainMenuButton("Back to the game", mainWindow->getAudioManager(), contentContainer);
    MainMenuButton* buttonSettings = new MainMenuButton("Settings", mainWindow->getAudioManager(), contentContainer);

    //Set buttons font
    buttonBackToMenu->setFont(buttonFont);
    buttonSettings->setFont(buttonFont);
    buttonBackToGame->setFont(buttonFont);

    connect(buttonBackToMenu, &QPushButton::clicked, this, &MainView::backToMenu);
    connect(buttonBackToGame, &QPushButton::clicked, this, &MainView::stopGamePaused);
    connect(buttonSettings, &QPushButton::clicked, settingsWidget, &SettingsWidget::show);

    //Score
    scoreManager->getActualScore()->setTimePlayed(scoreManager->getElapsedTimer()->elapsed() / 1000);
    int score = scoreManager->getActualScore()->getScore();
    int seconds = scoreManager->getActualScore()->getTimePlayed();
    QString timePlayed = QString::number(seconds / 60) + "m" + QString::number(seconds % 60) + "s";

    QLabel* scoreLabel = new QLabel(contentContainer);
    scoreLabel->setText(QString::number(score) + " pts - " + timePlayed);
    scoreLabel->setStyleSheet("color: white;");
    scoreLabel->setAlignment(Qt::AlignCenter);
    scoreLabel->setFont(scoreFont);

    //Creation buttons layout
    QVBoxLayout* buttonsLayout = new QVBoxLayout();
    buttonsLayout->setSpacing(this->window()->height() * 0.04);
    buttonsLayout->setContentsMargins(0, this->window()->height() * 0.05, 0, 0);
    buttonsLayout->addWidget(buttonSettings, Qt::AlignHCenter);
    buttonsLayout->addWidget(buttonBackToMenu, Qt::AlignHCenter);
    buttonsLayout->addWidget(buttonBackToGame, Qt::AlignHCenter);

    //Creation container layout
    QVBoxLayout* containerLayout = new QVBoxLayout(contentContainer);
    containerLayout->addWidget(titlePause, Qt::AlignHCenter);
    containerLayout->addWidget(scoreLabel, Qt::AlignHCenter);
    containerLayout->addLayout(buttonsLayout, Qt::AlignCenter);

    contentContainer->setLayout(containerLayout);
    contentContainer->adjustSize();
    contentContainer->show();


    //Size of buttons
    buttonBackToMenu->setFixedSize(buttonBackToMenu->size().width() * 1.15, buttonBackToMenu->size().height() * 2);
    buttonBackToGame->setFixedSize(buttonBackToGame->size().width() * 1.15, buttonBackToGame->size().height() * 2);
    buttonSettings->setFixedSize(buttonSettings->size().width() * 1.15, buttonSettings->size().height() * 2);
    containerLayout->activate();
    contentContainer->adjustSize();
    contentContainer->move((pauseScreen->width() - contentContainer->width()) / 2, (pauseScreen->height() - contentContainer->height()) / 2);


    //Fade of other content
    QGraphicsOpacityEffect* fadeContentEffect = new QGraphicsOpacityEffect(contentContainer);
    contentContainer->setGraphicsEffect(fadeContentEffect);
    QPropertyAnimation* animationContent = new QPropertyAnimation(fadeContentEffect, "opacity", contentContainer);
    animationContent->setDuration(250);
    animationContent->setStartValue(0);
    animationContent->setEndValue(1);
    animationContent->start(QAbstractAnimation::DeleteWhenStopped);
}

void MainView::updatePauseScreenSize(QSize size){
    pauseScreen->resize(size);
    settingsWidget->setFixedSize(QSize(this->width() * 0.6, 1)); //1 because the resizeEvent keep the ratio and only work with the width
    settingsWidget->move((pauseScreen->width() - settingsWidget->width()) / 2, (pauseScreen->height() - settingsWidget->height()) / 2);
    blackBackground->setGeometry(pauseScreen->rect());

    createContentPauseContainer();
}

void MainView::stopGamePaused(){
    delete pauseScreen;
    pauseScreen = nullptr;
    contentContainer = nullptr;
    settingsWidget = nullptr;
    mainWindow->getGameSene()->reverseGamePaused();
}

void MainView::backToMenu() {
    delete pauseScreen;
    pauseScreen = nullptr;
    contentContainer = nullptr;
    settingsWidget = nullptr;
    //Save score
    scoreManager->getActualScore()->setTimePlayed(scoreManager->getElapsedTimer()->elapsed() / 1000);
    scoreManager->getActualScore()->setDate(QDateTime::currentDateTime().toString("dd/MM/yyyy"));
    scoreManager->addScore(*(scoreManager->getActualScore()));
    //Go to menu
    mainWindow->goToStartMenu();
}
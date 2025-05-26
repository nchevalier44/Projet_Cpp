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

//Constructor
MainView::MainView(MainWindow* mainWindow, ScoreManager* scoreManager, QWidget* parent) : mainWindow(mainWindow), scoreManager(scoreManager), QGraphicsView(parent) {
    //Hide scrollbar (if there are)
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    //Initialize lastSize with -1 at the start
    lastSize.setWidth(-1);
    lastSize.setHeight(-1);

    //Fit the view at the scene rect at the start (with a little delay to let startMenuScene be created)
    QTimer::singleShot(50, this, [this]() {
        this->fitInView(sceneRect(), Qt::KeepAspectRatio);
    });
}

//Destructor
MainView::~MainView() {}

//Delete death screen and set pointer to nullptr
void MainView::deleteDeathScreen(){
    if(deathScreen != nullptr){
        delete deathScreen;
        deathScreen = nullptr;
    }
}

//Delete win screen and set pointer to nullptr
void MainView::deleteWinScreen(){
    if(winScreen != nullptr) {
        delete winScreen;
        winScreen = nullptr;
    }
}

//Redefinition of resizeEvent for the view keep the right size
void MainView::resizeEvent (QResizeEvent* event){

    //Don't update the view if the size change less that 5 pixels
    int deltaW = std::abs(lastSize.width() - event->size().width());
    int deltaH = std::abs(lastSize.height() - event->size().height());
    if (deltaW < 5 && deltaH < 5) return;

    //Call the initial resizeEvent
    QGraphicsView::resizeEvent(event);

    if(event->oldSize() != event->size()){

        //We want to have the dimension of the view equal to the scene only in the menu but no in game
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

            //Update size and position of pauseScreen elements
            if(pauseScreen){
                updatePauseScreenSize(event->size());
            }

            //Update size and position of HUD elements
            HUD* hud = mainWindow->getHUD();
            if(hud){
                QPointF windowSize(this->width(), this->height());
                Player* p = mainWindow->getGameSene()->getCharacter();
                if(p) {
                    hud->updateHUD(p->getHp(), hud->getHPWidget()->getMaxLife(), windowSize);
                }
            }
        }

        //keep in memory the lastSize
        lastSize = event->size();
    }

    //Update geometry of death screen or win screen, if exist
    if(deathScreen != nullptr){
        deathScreen->setGeometry(this->rect());
    }
    if(winScreen != nullptr){
        winScreen->setGeometry(this->rect());
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

    //Create death screen
    deathScreen = new QWidget(this);
    deathScreen->setGeometry(this->rect());
    deathScreen->show();

    //Creation of black background
    QWidget* blackBackground = new QWidget(deathScreen);
    blackBackground->setStyleSheet("background-color: black;");
    blackBackground->setGeometry(deathScreen->rect());
    blackBackground->show();

    //Create content container
    QWidget* contentContainer = new QWidget(deathScreen);

    //Creation of title and buttons
    QLabel* title = new QLabel("Game over !", contentContainer);
    title->setStyleSheet("color: white;");
    title->setAlignment(Qt::AlignCenter);
    title->setFont(titleFont);
    title->adjustSize();

    QPushButton* buttonRestart = new QPushButton("Restart the game", contentContainer);
    buttonRestart->setFont(buttonFont);
    connect(buttonRestart, &QPushButton::clicked, this, &MainView::startGameRequested);
    QPushButton* buttonBackToMenu = new QPushButton("Back to the menu", contentContainer);
    buttonBackToMenu->setFont(buttonFont);
    connect(buttonBackToMenu, &QPushButton::clicked, this, &MainView::goToStartMenuRequested);

    //Scores
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
    //Delete win screen if already exist (to not have 2 win screen created at the same time)
    deleteWinScreen();

    //Add font Jersey10 (pixel art)
    QFont buttonFont(FontManager::fontFamily);
    QFont titleFont(FontManager::fontFamily);
    QFont secondTitleFont(FontManager::fontFamily);
    QFont scoreFont(FontManager::fontFamily);
    buttonFont.setPixelSize(this->window()->height()*0.07); //setPixelSize set the pixel number of the height of a line (letter + margins)
    titleFont.setPixelSize(this->window()->height()*0.15);
    secondTitleFont.setPixelSize(this->window()->height()*0.05);
    scoreFont.setPixelSize(this->window()->height()*0.05);


    //Create win screen widget
    winScreen = new QWidget(this);
    winScreen->setGeometry(this->rect());
    winScreen->show();

    //Creation of black background
    QWidget* greenBackground = new QWidget(winScreen);
    greenBackground->setStyleSheet("background-color: green;");
    greenBackground->setGeometry(winScreen->rect());
    greenBackground->show();

    //Create content container
    QWidget* contentContainer = new QWidget(winScreen);

    //Creation of titles and buttons
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
    connect(buttonRestart, &QPushButton::clicked, this, &MainView::startGameRequested);
    QPushButton* buttonBackToMenu = new QPushButton("Back to the menu", contentContainer);
    buttonBackToMenu->setFont(buttonFont);
    connect(buttonBackToMenu, &QPushButton::clicked, this, &MainView::goToStartMenuRequested);

    //Scores
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
    contentContainer->move((winScreen->width() - contentContainer->width()) / 2,
                           (winScreen->height() - contentContainer->height()) / 2);


    //Fade of black background
    QGraphicsOpacityEffect* fadeBackgroundEffect = new QGraphicsOpacityEffect(greenBackground);
    greenBackground->setGraphicsEffect(fadeBackgroundEffect);
    QPropertyAnimation* animationBackground = new QPropertyAnimation(fadeBackgroundEffect, "opacity", greenBackground);
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
    //Delete pause screen if already exist (it's for when we update it)
    if(pauseScreen){
        delete pauseScreen;
        pauseScreen = nullptr;
    }

    //Create pause screen widget
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

    //Change the size of the window when we change the text
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

    //Shadow effect
    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect(settingsWidget);
    shadowEffect->setOffset(0, 0);
    shadowEffect->setBlurRadius(50);
    shadowEffect->setColor(Qt::black);
    settingsWidget->setGraphicsEffect(shadowEffect);

    //Create all the contents
    createContentPauseContainer();

    //Fade animation of black background
    QGraphicsOpacityEffect* fadeBackgroundEffect = new QGraphicsOpacityEffect(blackBackground);
    blackBackground->setGraphicsEffect(fadeBackgroundEffect);
    QPropertyAnimation* animationBackground = new QPropertyAnimation(fadeBackgroundEffect, "opacity", blackBackground);
    animationBackground->setDuration(250);
    animationBackground->setStartValue(0);
    animationBackground->setEndValue(0.3);
    animationBackground->start(QAbstractAnimation::DeleteWhenStopped);
}


void MainView::createContentPauseContainer(){
    //Delete the content container if already exist (it's for when we update it)
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

    //Buttons actions
    connect(buttonBackToMenu, &QPushButton::clicked, this, &MainView::backToMenu);
    connect(buttonBackToGame, &QPushButton::clicked, this, &MainView::stopGamePaused);
    connect(buttonSettings, &QPushButton::clicked, settingsWidget, &SettingsWidget::show);

    //Score
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
    //Size and postion of contentContainer
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
    //Update pauseScreen size and its elements
    pauseScreen->resize(size);
    settingsWidget->setFixedSize(QSize(this->width() * 0.6, 1)); //1 because the resizeEvent keep the ratio and only work with the width
    settingsWidget->move((pauseScreen->width() - settingsWidget->width()) / 2, (pauseScreen->height() - settingsWidget->height()) / 2);
    blackBackground->setGeometry(pauseScreen->rect());

    createContentPauseContainer();
}

void MainView::stopGamePaused(){
    //Delete pause screen and the elements in it
    delete pauseScreen;
    pauseScreen = nullptr;
    contentContainer = nullptr;
    settingsWidget = nullptr;

    //Active the timer and movies that are stopped
    mainWindow->getGameSene()->reverseGamePaused();
}

void MainView::backToMenu() {
    //Delete pause screen and the elements in it
    delete pauseScreen;
    pauseScreen = nullptr;
    contentContainer = nullptr;
    settingsWidget = nullptr;

    //Go to menu
    mainWindow->goToStartMenu();
}
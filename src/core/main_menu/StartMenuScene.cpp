#include "StartMenuScene.h"
#include "../MainWindow.h"
#include "../FontManager.h"

StartMenuScene::StartMenuScene(MainWindow* mainWindow, QObject* parent) : QGraphicsScene(parent) {

    //Add background music
    audioPlayer = new QMediaPlayer(this);
    QAudioOutput* audioOutput = new QAudioOutput(this);
    audioOutput->setVolume(50);
    audioPlayer->setAudioOutput(audioOutput);
    audioPlayer->setSource(QUrl::fromLocalFile(PATH_MAIN_MENU_MUSIC));
    audioPlayer->setLoops(QMediaPlayer::Infinite);
    audioPlayer->play();
    mainWindow->getAudioManager()->addMusicObject(audioOutput, audioOutput->volume());

    //Add background image
    this->background = new QPixmap();
    this->background->load(PATH_MAIN_MENU_BACKGROUND);
    this->setSceneRect(0, 0, background->width(), background->height());
    mainWindow->setBackgroundRatio(float(background->width()) / float(background->height()));

    //Create buttons
    createButtons(mainWindow);

    //Create title label
    /*QFont titleFont(FontManager::fontFamily, 65);
    titleLabel = new QLabel("Title of the game");
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: white;");
    titleLabel->setAttribute(Qt::WA_OpaquePaintEvent);

    //Add title to the scene
    QGraphicsProxyWidget* proxyTitleContainer = this->addWidget(titleLabel);
    qreal posXTitle = (this->width() - titleLabel->width()) / 2;
    qreal posYTitle = (this->width() - titleLabel->height()) / 2 - 0.33 * height() - titleLabel->height();
    proxyTitleContainer->setPos(posXTitle, posYTitle);
    */
}

//Destructor
StartMenuScene::~StartMenuScene() {
    delete background;
    background = nullptr;
    delete settingsProxyWidget;
    settingsProxyWidget = nullptr;
    delete buttonsContainer;
    buttonsContainer = nullptr;
    delete titleLabel;
    titleLabel = nullptr;
    delete scoreboardWidget;
    scoreboardWidget = nullptr;
}


void StartMenuScene::drawBackground(QPainter* painter, const QRectF& rect) {
    Q_UNUSED(rect);
    painter->drawPixmap(QPointF(0,0), *background, sceneRect());
}

void StartMenuScene::createSettingsWidget(MainWindow* mainWindow){
    if (settingsProxyWidget) {
        if (settingsProxyWidget->scene() == this) {
            this->removeItem(settingsProxyWidget);
        }
        settingsProxyWidget->setWidget(nullptr); // <-- évite que le proxy le delete
        delete settingsProxyWidget;
        settingsProxyWidget = nullptr;
    }

    if (settingsWidget) {
        delete settingsWidget;
        settingsWidget = nullptr;
    }

    settingsWidget = new SettingsWidget(mainWindow);
    settingsWidget->setFixedSize(settingsWidget->size() * 1.25);

    //Add settings widget to the scene
    settingsProxyWidget = new QGraphicsProxyWidget();
    settingsProxyWidget->setWidget(settingsWidget);
    this->addItem(settingsProxyWidget);
    settingsProxyWidget->hide();
    settingsProxyWidget->setZValue(100);
    settingsProxyWidget->setPos(background->width()/2 - settingsWidget->width()/2, background->height()/2 - settingsWidget->height()/2);

    //Add shadow effect to the settings widget
    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect(settingsProxyWidget);
    shadowEffect->setOffset(0, 0);
    shadowEffect->setBlurRadius(50);
    shadowEffect->setColor(Qt::black);
    settingsProxyWidget->setGraphicsEffect(shadowEffect);
}

void StartMenuScene::createScoreboardWidget(MainWindow* mainWindow){
    if (scoreboardProxyWidget) {
        if (scoreboardProxyWidget->scene() == this) {
            this->removeItem(scoreboardProxyWidget);
        }
        scoreboardProxyWidget->setWidget(nullptr); // <-- évite que le proxy le delete
        delete scoreboardProxyWidget;
        scoreboardProxyWidget = nullptr;
    }

    if (scoreboardWidget) {
        delete scoreboardWidget;
        scoreboardWidget = nullptr;
    }


    scoreboardWidget = new ScoreboardWidget(mainWindow);

    //Add settings widget to the scene
    scoreboardProxyWidget = new QGraphicsProxyWidget();
    scoreboardProxyWidget->setWidget(scoreboardWidget);
    this->addItem(scoreboardProxyWidget);
    scoreboardProxyWidget->hide();
    scoreboardProxyWidget->setZValue(100);
    scoreboardProxyWidget->setPos(background->width()/2 - scoreboardWidget->width()/2, background->height()/2 - scoreboardWidget->height()/2);

    //Add shadow effect to the settings widget
    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect(scoreboardWidget);
    shadowEffect->setOffset(0, 0);
    shadowEffect->setBlurRadius(50);
    shadowEffect->setColor(Qt::black);
    scoreboardProxyWidget->setGraphicsEffect(shadowEffect);
}

void StartMenuScene::createButtons(MainWindow* mainWindow){
    //Create buttons container and buttons
    if(buttonsContainer){
        delete buttonsContainer;
        buttonsContainer = nullptr;
    }
    buttonsContainer = new QWidget();
    buttonsContainer->setAttribute(Qt::WA_OpaquePaintEvent); //get background of the widget transparent
    MainMenuButton* startButton = new MainMenuButton("Start", buttonsContainer);
    MainMenuButton* settingsButton = new MainMenuButton("Options", buttonsContainer);
    MainMenuButton* scoreboardButton = new MainMenuButton("Scoreboard", buttonsContainer);
    MainMenuButton* exitButton = new MainMenuButton("Exit", buttonsContainer);

    //Create buttons layout
    QVBoxLayout* buttonsLayout = new QVBoxLayout(buttonsContainer);
    buttonsLayout->setSpacing(height() * 0.01); // 0.035
    buttonsLayout->addWidget(startButton);
    buttonsLayout->addWidget(settingsButton);
    buttonsLayout->addWidget(scoreboardButton);
    buttonsLayout->addWidget(exitButton);

    //Set buttons font
    QFont buttonFont(FontManager::fontFamily, 33);
    startButton->setFont(buttonFont);
    settingsButton->setFont(buttonFont);
    scoreboardButton->setFont(buttonFont);
    exitButton->setFont(buttonFont);

    startButton->setFixedSize(startButton->size() * 4);
    settingsButton->setFixedSize(settingsButton->size() * 4);
    scoreboardButton->setFixedSize(scoreboardButton->size() * 4);
    exitButton->setFixedSize(exitButton->size() * 4);


    //Add sound to buttons
    sound = new QSoundEffect(buttonsContainer);
    sound->setSource(QUrl::fromLocalFile(PATH_MAIN_MENU_BUTTON_SOUND));
    sound->setVolume(20);
    mainWindow->getAudioManager()->addSFXObject(sound, sound->volume());


    createSettingsWidget(mainWindow);
    createScoreboardWidget(mainWindow);


    //Set buttons actions
    QObject::connect(startButton, &QPushButton::clicked, this, &StartMenuScene::startGameRequested);
    QObject::connect(settingsButton, &QPushButton::clicked, settingsWidget, &SettingsWidget::show);
    QObject::connect(scoreboardButton, &QPushButton::clicked, scoreboardWidget, &ScoreboardWidget::show);
    QObject::connect(exitButton, &QPushButton::clicked, this, &QApplication::quit);

    QObject::connect(startButton, &QPushButton::clicked, sound, &QSoundEffect::play);
    QObject::connect(settingsButton, &QPushButton::clicked, sound, &QSoundEffect::play);
    QObject::connect(scoreboardButton, &QPushButton::clicked, sound, &QSoundEffect::play);
    QObject::connect(exitButton, &QPushButton::clicked, sound, &QSoundEffect::play);

    //Add buttons container to the scene
    QGraphicsProxyWidget* proxyButtonsContainer = this->addWidget(buttonsContainer);
    qreal posXButtons = (this->width() - buttonsContainer->width()) / 2;
    qreal posYButtons = (this->height() - buttonsContainer->height()) / 2 + 250;
    proxyButtonsContainer->setPos(posXButtons, posYButtons);

}
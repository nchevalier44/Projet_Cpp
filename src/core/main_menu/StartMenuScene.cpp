#include "StartMenuScene.h"
#include "../MainWindow.h"
#include "../FontManager.h"

StartMenuScene::StartMenuScene(MainWindow* mainWindow, QObject* parent) : QGraphicsScene(parent) {

    //Add background music
    audioPlayer = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    audioOutput->setVolume(0.5);
    audioPlayer->setAudioOutput(audioOutput);
    connect(audioPlayer, &QMediaPlayer::mediaStatusChanged, audioPlayer, [=]() {
        if (audioPlayer->mediaStatus() == QMediaPlayer::LoadedMedia) {
            audioPlayer->play();
        }
    });
    audioPlayer->setSource(QUrl::fromLocalFile(PATH_MAIN_MENU_MUSIC));
    audioPlayer->setLoops(QMediaPlayer::Infinite);
    mainWindow->getAudioManager()->addMusicObject(audioOutput, audioOutput->volume());

    //Add background image
    this->background = new QPixmap();
    this->background->load(PATH_MAIN_MENU_BACKGROUND);
    this->setSceneRect(0, 0, background->width(), background->height());
    mainWindow->setBackgroundRatio(float(background->width()) / float(background->height()));

    //Create buttons
    createButtons(mainWindow);
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
    //We delete it if it exist, before recreate it (it's for update)
    if (settingsProxyWidget) {
        if (settingsProxyWidget->scene() == this) {
            this->removeItem(settingsProxyWidget);
        }
        settingsProxyWidget->setWidget(nullptr);
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
    //We delete it if it exist, before recreate it (it's for update)
    if (scoreboardProxyWidget) {
        if (scoreboardProxyWidget->scene() == this) {
            this->removeItem(scoreboardProxyWidget);
        }
        scoreboardProxyWidget->setWidget(nullptr);
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
    //We delete it if it exist, before recreate it
    if(buttonsContainer){
        delete buttonsContainer;
        buttonsContainer = nullptr;
    }

    //Create buttons container and buttons
    buttonsContainer = new QWidget();
    buttonsContainer->setAttribute(Qt::WA_OpaquePaintEvent); //get background of the widget transparent
    MainMenuButton* startButton = new MainMenuButton("Start", mainWindow->getAudioManager(), buttonsContainer);
    MainMenuButton* settingsButton = new MainMenuButton("Options", mainWindow->getAudioManager(), buttonsContainer);
    MainMenuButton* scoreboardButton = new MainMenuButton("Scoreboard", mainWindow->getAudioManager(), buttonsContainer);
    MainMenuButton* exitButton = new MainMenuButton("Exit", mainWindow->getAudioManager(), buttonsContainer);

    //Create buttons layout
    QVBoxLayout* buttonsLayout = new QVBoxLayout(buttonsContainer);
    buttonsLayout->setSpacing(height() * 0.01);
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

    //Set the right size
    startButton->setFixedSize(startButton->size() * 4);
    settingsButton->setFixedSize(settingsButton->size() * 4);
    scoreboardButton->setFixedSize(scoreboardButton->size() * 4);
    exitButton->setFixedSize(exitButton->size() * 4);


    createSettingsWidget(mainWindow);
    createScoreboardWidget(mainWindow);


    //Set buttons actions
    QObject::connect(startButton, &QPushButton::clicked, this, &StartMenuScene::startGameRequested);
    QObject::connect(startButton, &QPushButton::clicked, audioPlayer, &QMediaPlayer::pause);
    QObject::connect(settingsButton, &QPushButton::clicked, settingsWidget, &SettingsWidget::show);
    QObject::connect(scoreboardButton, &QPushButton::clicked, scoreboardWidget, &ScoreboardWidget::show);
    QObject::connect(exitButton, &QPushButton::clicked, this, &QApplication::quit);

    //Add buttons container to the scene
    QGraphicsProxyWidget* proxyButtonsContainer = this->addWidget(buttonsContainer);
    qreal posXButtons = (this->width() - buttonsContainer->width()) / 2;
    qreal posYButtons = (this->height() - buttonsContainer->height()) / 2 + 250;
    proxyButtonsContainer->setPos(posXButtons, posYButtons);

}
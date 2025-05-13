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

    //Create scores
    createScores(mainWindow);

    //Create title label
    QFont titleFont(FontManager::fontFamily, 65);
    titleLabel = new QLabel("Title of the game");
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: white;");
    titleLabel->setAttribute(Qt::WA_OpaquePaintEvent);

    //Add title to the scene
    QGraphicsProxyWidget* proxyTitleContainer = this->addWidget(titleLabel);
    qreal posXTitle = (this->width() - titleLabel->width()) / 2;
    qreal posYTitle = (this->width() - titleLabel->height()) / 2 - 0.33 * height() - titleLabel->height();
    proxyTitleContainer->setPos(posXTitle, posYTitle);

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
    delete scoresContainer;
    scoresContainer = nullptr;
}


void StartMenuScene::drawBackground(QPainter* painter, const QRectF& rect) {
    Q_UNUSED(rect);
    painter->drawPixmap(QPointF(0,0), *background, sceneRect());
}

void StartMenuScene::createSettingsWidget(MainWindow* mainWindow){
    settingsWidget = new SettingsWidget(mainWindow);

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

void StartMenuScene::createButtons(MainWindow* mainWindow){
    //Create buttons container and buttons
    buttonsContainer = new QWidget();
    buttonsContainer->setAttribute(Qt::WA_OpaquePaintEvent); //get background of the widget transparent
    MainMenuButton* startButton = new MainMenuButton("Start", buttonsContainer);
    MainMenuButton* settingsButton = new MainMenuButton("Options", buttonsContainer);
    MainMenuButton* exitButton = new MainMenuButton("Exit", buttonsContainer);

    //Create buttons layout
    QVBoxLayout* buttonsLayout = new QVBoxLayout(buttonsContainer);
    buttonsLayout->setSpacing(height() * 0.05);
    buttonsLayout->addWidget(startButton);
    buttonsLayout->addWidget(settingsButton);
    buttonsLayout->addWidget(exitButton);

    //Set buttons font
    QFont buttonFont(FontManager::fontFamily, 40);
    startButton->setFont(buttonFont);
    settingsButton->setFont(buttonFont);
    exitButton->setFont(buttonFont);

    //Add sound to buttons
    sound = new QSoundEffect(buttonsContainer);
    sound->setSource(QUrl::fromLocalFile(PATH_MAIN_MENU_BUTTON_SOUND));
    sound->setVolume(20);
    mainWindow->getAudioManager()->addSFXObject(sound, sound->volume());


    createSettingsWidget(mainWindow);


    //Set buttons actions
    QObject::connect(startButton, &QPushButton::clicked, this, &StartMenuScene::startGameRequested);
    QObject::connect(settingsButton, &QPushButton::clicked, settingsWidget, &SettingsWidget::show);
    QObject::connect(exitButton, &QPushButton::clicked, this, &QApplication::quit);

    QObject::connect(startButton, &QPushButton::clicked, sound, &QSoundEffect::play);
    QObject::connect(settingsButton, &QPushButton::clicked, sound, &QSoundEffect::play);
    QObject::connect(exitButton, &QPushButton::clicked, sound, &QSoundEffect::play);

    //Add buttons container to the scene
    QGraphicsProxyWidget* proxyButtonsContainer = this->addWidget(buttonsContainer);
    qreal posXButtons = (this->width() - buttonsContainer->width()) / 4;
    qreal posYButtons = (this->height() - buttonsContainer->height()) / 2 + 100;
    proxyButtonsContainer->setPos(posXButtons, posYButtons);

}

void StartMenuScene::createScores(MainWindow* mainWindow) {
    //Font
    QFont font(FontManager::fontFamily, 35);

    //Recreate the scores container
    if(scoresContainer) delete scoresContainer; ///Delete the previous score container
    scoresContainer = new QWidget();
    QVBoxLayout* scoresLayout = new QVBoxLayout(scoresContainer);

    //Title
    QLabel* title = new QLabel("Best Scores", scoresContainer);
    title->setFont(font);
    title->setStyleSheet("color: white;");
    title->setAlignment(Qt::AlignHCenter);
    scoresLayout->addWidget(title);

    //Get scores
    mainWindow->getScoreManager()->loadScores();
    QList<Score> scores = mainWindow->getScoreManager()->getBestScoresList();
    int n = scores.size();

    if(n == 0){ // if no scores
        QLabel* label = new QLabel("No previous scores...", scoresContainer);
        label->setFont(font);
        label->setStyleSheet("color: white;");
        scoresLayout->addWidget(label);
    } else{ //if scores exist display each scores
        for (int i = n - 1; i >= 0; i--) {
            QString number = QString::number(n - i);
            QString score = QString::number(scores[i].getScore());
            int seconds = scores[i].getTimePlayed();
            QString timePlayed = QString::number(seconds / 60) + "m" + QString::number(seconds % 60) + "s";
            QString date = scores[i].getDate();

            QLabel *label = new QLabel(number + " | " + score + " pts - " + timePlayed + " - " + date, scoresContainer);
            label->setFont(font);
            label->setStyleSheet("color: white;");
            scoresLayout->addWidget(label);
        }
    }
    scoresContainer->setLayout(scoresLayout);
    scoresContainer->setStyleSheet("background-color: transparent;");

    //Place scores
    QGraphicsProxyWidget* proxyScoresContainer = this->addWidget(scoresContainer);
    qreal posXScores = 3.5 * (this->width() - scoresContainer->width()) / 4;
    qreal posYScores = (this->height() - scoresContainer->height()) / 2 + 100;
    proxyScoresContainer->setPos(posXScores, posYScores);
}
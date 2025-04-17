#include "StartMenuScene.h"
#include "WindowSizeComboBox.h"
#include "../MainWindow.h"
#include <QTimer>
#include <QStyleFactory>
#include <QGraphicsDropShadowEffect>

StartMenuScene::StartMenuScene(MainWindow* mainWindow, QObject* parent) : QGraphicsScene(parent) {

    //Add font
    int fontId = QFontDatabase::addApplicationFont(PATH_JERSEY10_FONT);
    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont buttonFont(fontFamily, 40);
    QFont titleFont(fontFamily, 65);

    //Add background music
    audioPlayer = new QMediaPlayer(this);
    QAudioOutput* audioOutput = new QAudioOutput(this);
    audioOutput->setVolume(50 * mainWindow->getVolumePercentage() / 100);
    audioPlayer->setAudioOutput(audioOutput);
    audioPlayer->setSource(QUrl::fromLocalFile(PATH_MAIN_MENU_MUSIC));
    audioPlayer->setLoops(QMediaPlayer::Infinite);
    audioPlayer->play();

    //Add background image
    this->background = new QPixmap();
    this->background->load(PATH_MAIN_MENU_BACKGROUND);
    this->setSceneRect(0, 0, background->width(), background->height());
    mainWindow->setBackgroundRatio(float(background->width()) / float(background->height()));

    //Create buttons container and buttons
    buttonsContainer = new QWidget();
    buttonsContainer->setAttribute(Qt::WA_OpaquePaintEvent); //get background of the widget transparent
    MainMenuButton* startButton = new MainMenuButton("Start");
    MainMenuButton* settingsButton = new MainMenuButton("Options");
    MainMenuButton* exitButton = new MainMenuButton("Exit");

    //Create buttons layout
    QVBoxLayout* buttonsLayout = new QVBoxLayout(buttonsContainer);
    buttonsLayout->setSpacing(height() * 0.05);
    buttonsLayout->addWidget(startButton);
    buttonsLayout->addWidget(settingsButton);
    buttonsLayout->addWidget(exitButton);
    buttonsContainer->setLayout(buttonsLayout);

    //Set buttons font
    startButton->setFont(buttonFont);
    settingsButton->setFont(buttonFont);
    exitButton->setFont(buttonFont);

    //Add sound to buttons
    sound = new QSoundEffect();
    sound->setSource(QUrl::fromLocalFile(PATH_MAIN_MENU_BUTTON_SOUND));
    sound->setVolume(20 * mainWindow->getVolumePercentage() / 100);

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

    //Set buttons actions
    QObject::connect(startButton, &QPushButton::clicked, this, &StartMenuScene::startGameRequested);
    QObject::connect(settingsButton, &QPushButton::clicked, settingsWidget, &SettingsWidget::show);
    QObject::connect(exitButton, &QPushButton::clicked, this, &QApplication::quit);

    QObject::connect(startButton, &QPushButton::clicked, sound, &QSoundEffect::play);
    QObject::connect(settingsButton, &QPushButton::clicked, sound, &QSoundEffect::play);
    QObject::connect(exitButton, &QPushButton::clicked, sound, &QSoundEffect::play);

    //Add buttons container to the scene
    QGraphicsProxyWidget* proxyButtonsContainer = this->addWidget(buttonsContainer);
    qreal posXButtons = (this->width() - buttonsContainer->width()) / 2;
    qreal posYButtons = (this->height() - buttonsContainer->height()) / 2 + 100;
    proxyButtonsContainer->setPos(posXButtons, posYButtons);

    //Create title label
    QLabel* titleLabel = new QLabel("Title of the game");
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
    delete buttonsContainer;
    buttonsContainer = nullptr;
    delete settingsWidget;
    settingsWidget = nullptr;
    delete sound;
    sound = nullptr;
    delete audioPlayer;
    audioPlayer = nullptr;
    delete settingsProxyWidget;
    settingsProxyWidget = nullptr;
}


void StartMenuScene::drawBackground(QPainter* painter, const QRectF& rect) {
    Q_UNUSED(rect);
    painter->drawPixmap(QPointF(0,0), *background, sceneRect());
}
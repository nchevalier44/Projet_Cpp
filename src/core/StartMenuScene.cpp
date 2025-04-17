#include "StartMenuScene.h"
#include "WindowSizeComboBox.h"
#include "MainWindow.h"
#include <QTimer>
#include <QStyleFactory>
#include <QGraphicsDropShadowEffect>

StartMenuScene::StartMenuScene(MainWindow* mainWindow, QObject* parent) : QGraphicsScene(parent) {

    //Add font Jersey10 (pixel art)
    int fontId = QFontDatabase::addApplicationFont("../assets/fonts/Jersey10-Regular.ttf");
    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont buttonFont(fontFamily, 40);
    QFont titleFont(fontFamily, 65);

    //Add background music
    audioPlayer = new QMediaPlayer(this);
    QAudioOutput* audioOutput = new QAudioOutput(this);
    audioOutput->setVolume(50 * mainWindow->getVolumePercentage() / 100);
    audioPlayer->setAudioOutput(audioOutput);
    audioPlayer->setSource(QUrl::fromLocalFile("../assets/musics/start_menu_theme.wav"));
    audioPlayer->setLoops(QMediaPlayer::Infinite);
    audioPlayer->play();

    //Add background image
    this->background = new QPixmap();
    this->background->load("../assets/images/menu/background.png");
    this->setSceneRect(0, 0, background->width(), background->height());
    mainWindow->setBackgroundRatio(float(background->width()) / float(background->height()));

    //Add buttons
    buttonsContainer = new QWidget();
    QVBoxLayout* buttonsLayout = new QVBoxLayout(buttonsContainer);

    MainMenuButton* startButton = new MainMenuButton("Start");
    MainMenuButton* settingsButton = new MainMenuButton("Options");
    MainMenuButton* exitButton = new MainMenuButton("Exit");

    startButton->setFont(buttonFont);
    settingsButton->setFont(buttonFont);
    exitButton->setFont(buttonFont);

    settingsWidget = new SettingsWidget(mainWindow);

    QGraphicsProxyWidget* proxyWidget = new QGraphicsProxyWidget();
    proxyWidget->setWidget(settingsWidget);
    this->addItem(proxyWidget);
    proxyWidget->hide();
    proxyWidget->setZValue(100);
    proxyWidget->setPos(background->width()/2 - settingsWidget->width()/2, background->height()/2 - settingsWidget->height()/2);

    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect();
    shadowEffect->setOffset(0, 0);
    shadowEffect->setBlurRadius(50);
    shadowEffect->setColor(Qt::black);

    proxyWidget->setGraphicsEffect(shadowEffect);



    QObject::connect(startButton, &QPushButton::clicked, this, &StartMenuScene::startGameRequested);
    QObject::connect(settingsButton, &QPushButton::clicked, settingsWidget, &SettingsWidget::show);
    QObject::connect(exitButton, &QPushButton::clicked, this, &QApplication::quit);

    sound = new QSoundEffect();
    sound->setSource(QUrl::fromLocalFile("../assets/sounds_effects/button.wav"));
    sound->setVolume(20 * mainWindow->getVolumePercentage() / 100);

    QObject::connect(startButton, &QPushButton::clicked, sound, &QSoundEffect::play);
    QObject::connect(settingsButton, &QPushButton::clicked, sound, &QSoundEffect::play);
    QObject::connect(exitButton, &QPushButton::clicked, sound, &QSoundEffect::play);

    buttonsLayout->addWidget(startButton);
    buttonsLayout->addWidget(settingsButton);
    buttonsLayout->addWidget(exitButton);

    buttonsContainer->setLayout(buttonsLayout);

    buttonsContainer->setAttribute(Qt::WA_OpaquePaintEvent); //get background of the widget transparent
    buttonsLayout->setSpacing(50); //Spacing between buttons

    QGraphicsProxyWidget* proxyButtonsContainer = this->addWidget(buttonsContainer);
    qreal posXButtons = (this->width() - buttonsContainer->width()) / 2;
    qreal posYButtons = (this->height() - buttonsContainer->height()) / 2 + 100;
    proxyButtonsContainer->setPos(posXButtons, posYButtons);

    //Add title
    QLabel* titleLabel = new QLabel("Title of the game");
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: white;");
    titleLabel->setAttribute(Qt::WA_OpaquePaintEvent);
    QGraphicsProxyWidget* proxyTitleContainer = this->addWidget(titleLabel);

    qreal posXTitle = (this->width() - titleLabel->width()) / 2;
    qreal posYTitle = (this->width() - titleLabel->height()) / 2 - 0.33 * height() - titleLabel->height();
    proxyTitleContainer->setPos(posXTitle, posYTitle);

    qDebug() << "RATIO : " << float(proxyWidget->size().width()) << float(proxyWidget->size().height());


}

void StartMenuScene::drawBackground(QPainter* painter, const QRectF& rect) {
    Q_UNUSED(rect);
    painter->drawPixmap(QPointF(0,0), *background, sceneRect());
}
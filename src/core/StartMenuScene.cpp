#include "StartMenuScene.h"
#include <QTimer>
#include <QStyleOptionButton>

SettingsWidget::SettingsWidget(int* volume, QWidget* parent) : QWidget(parent), volumePercentage(volume) {
    //Title
    QLabel* title = new QLabel("Settings");

    //Volume widgets (slider) and layout
    QWidget* volumeWidget = new QWidget(this);
    QVBoxLayout* volumeLayout = new QVBoxLayout(volumeWidget);
    QLabel* volumeLabel = new QLabel("Volume", volumeWidget);
    volumeSlider = new VolumeSlider(Qt::Horizontal, volumeWidget);
    volumeLayout->addWidget(volumeLabel);
    volumeLayout->addWidget(volumeSlider);
    volumeWidget->setLayout(volumeLayout);

    //Window size widgets (dropdown) and layout
    QWidget* windowSizeWidget = new QWidget(this);
    QVBoxLayout* windowSizeLayout = new QVBoxLayout(windowSizeWidget);
    QLabel* windowSizeLabel = new QLabel("Window size", windowSizeWidget);
    windowSizeDropdown = new QComboBox(windowSizeWidget);
    windowSizeLayout->addWidget(windowSizeLabel);
    windowSizeLayout->addWidget(windowSizeDropdown);
    windowSizeWidget->setLayout(windowSizeLayout);

    //Button (cancel and save) widgets and layout
    QWidget* buttonsWidget = new QWidget(this);
    QHBoxLayout* buttonsLayout = new QHBoxLayout(buttonsWidget);
    QPushButton* cancelButton = new QPushButton("Cancel", buttonsWidget);
    QPushButton* saveButton = new QPushButton("Save", buttonsWidget);
    buttonsLayout->addWidget(cancelButton);
    buttonsLayout->addWidget(saveButton);
    buttonsWidget->setLayout(buttonsLayout);

    //Container layout (of SettingsWidget)
    QVBoxLayout* containerLayout = new QVBoxLayout(this);
    containerLayout->addWidget(title);
    containerLayout->addWidget(volumeWidget);
    containerLayout->addWidget(windowSizeWidget);
    containerLayout->addWidget(buttonsWidget);
    this->setLayout(containerLayout);

    resetValues();

    QObject::connect(cancelButton, &QPushButton::clicked, this, &SettingsWidget::cancelSettings);
    QObject::connect(saveButton, &QPushButton::clicked, this, &SettingsWidget::saveSettings);
}

StartMenuScene::StartMenuScene(int* volume, QObject* parent) : QGraphicsScene(parent), volumePercentage(volume){

    //Add font Jersey10 (pixel art)
    int fontId = QFontDatabase::addApplicationFont("../assets/fonts/Jersey10-Regular.ttf");
    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont buttonFont(fontFamily, 40);
    QFont titleFont(fontFamily, 65);

    //Add background music
    audioPlayer = new QMediaPlayer(this);
    QAudioOutput* audioOutput = new QAudioOutput(this);
    audioOutput->setVolume(50);
    audioPlayer->setAudioOutput(audioOutput);
    audioPlayer->setSource(QUrl::fromLocalFile("../assets/musics/start_menu_theme.wav"));
    audioPlayer->setLoops(QMediaPlayer::Infinite);
    audioPlayer->play();

    //Add background image
    this->background = new QPixmap();
    this->background->load("../assets/images/menu/background_start_menu.png");
    this->setSceneRect(0, 0, background->width(), background->height());

    //Add buttons
    buttonsContainer = new QWidget();
    QVBoxLayout* buttonsLayout = new QVBoxLayout(buttonsContainer);

    MainMenuButton* startButton = new MainMenuButton("Start");
    MainMenuButton* settingsButton = new MainMenuButton("Options");
    MainMenuButton* exitButton = new MainMenuButton("Exit");

    startButton->setFont(buttonFont);
    settingsButton->setFont(buttonFont);
    exitButton->setFont(buttonFont);

    startButton->setImagePath("../assets/images/menu/Mainmenu_button.png");
    startButton->setGifPath("../assets/images/menu/Mainmenu_buttonGif.gif");
    settingsButton->setImagePath("../assets/images/menu/Mainmenu_button.png");
    settingsButton->setGifPath("../assets/images/menu/Mainmenu_buttonGif.gif");
    exitButton->setImagePath("../assets/images/menu/Mainmenu_button.png");
    exitButton->setGifPath("../assets/images/menu/Mainmenu_buttonGif.gif");

    settingsWidget = new SettingsWidget(volumePercentage);

    QObject::connect(startButton, &QPushButton::clicked, this, &StartMenuScene::startGameRequested);
    QObject::connect(settingsButton, &QPushButton::clicked, settingsWidget, &SettingsWidget::show);
    QObject::connect(exitButton, &QPushButton::clicked, this, &QApplication::quit);

    sound = new QSoundEffect();
    sound->setSource(QUrl::fromLocalFile("../assets/sounds_effects/button.wav"));
    sound->setVolume(20);

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
    titleLabel->setAttribute(Qt::WA_OpaquePaintEvent);
    QGraphicsProxyWidget* proxyTitleContainer = this->addWidget(titleLabel);

    qreal posXTitle = (this->width() - titleLabel->width()) / 2;
    qreal posYTitle = (this->width() - titleLabel->height()) / 2 - 600;
    proxyTitleContainer->setPos(posXTitle, posYTitle);

}

void StartMenuScene::drawBackground(QPainter* painter, const QRectF& rect) {
    Q_UNUSED(rect);
    painter->drawPixmap(QPointF(0,0), *background, sceneRect());
}

void SettingsWidget::resetValues(){
    volumeSlider->setValue(*volumePercentage);
}

void SettingsWidget::cancelSettings() {
    resetValues();
    hide();
}

void SettingsWidget::saveSettings(){
    *volumePercentage = volumeSlider->value();
    hide();
}
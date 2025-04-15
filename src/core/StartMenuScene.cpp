#include "StartMenuScene.h"
#include <QTimer>
#include <QStyleOptionButton>

StartMenuScene::StartMenuScene(QObject* parent) : QGraphicsScene(parent){


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



    MenuButton* startButton = new MenuButton("Start");
    MenuButton* optionsButton = new MenuButton("Options");
    MenuButton* exitButton = new MenuButton("Exit");

    startButton->setFont(buttonFont);
    optionsButton->setFont(buttonFont);
    exitButton->setFont(buttonFont);

    startButton->setImagePath("../assets/images/menu/Mainmenu_button.png");
    optionsButton->setImagePath("../assets/images/menu/Mainmenu_button.png");
    exitButton->setImagePath("../assets/images/menu/Mainmenu_button.png");

    //Scale the button


    QObject::connect(startButton, &QPushButton::clicked, this, &StartMenuScene::startGameRequested);
    //TO DO : click on options button
    QObject::connect(exitButton, &QPushButton::clicked, this, &QApplication::quit);

    sound = new QSoundEffect();
    sound->setSource(QUrl::fromLocalFile("../assets/sounds_effects/button.wav"));
    sound->setVolume(20);

    QObject::connect(startButton, &QPushButton::clicked, sound, &QSoundEffect::play);
    QObject::connect(optionsButton, &QPushButton::clicked, sound, &QSoundEffect::play);
    QObject::connect(exitButton, &QPushButton::clicked, sound, &QSoundEffect::play);

    buttonsLayout->addWidget(startButton);
    buttonsLayout->addWidget(optionsButton);
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

void MenuButton::paintEvent(QPaintEvent* event) {
    QPainter painter(this);

    // Dessiner l'image de fond redimensionnée
    if (!background.isNull()) {
        painter.drawPixmap(rect(), background);
    }

    // Appeler le paintEvent de QPushButton pour dessiner le texte
    QPushButton::paintEvent(event);

    QStyleOptionButton option;
    option.initFrom(this);
    option.text = this->text();
    option.icon = this->icon();
    style()->drawControl(QStyle::CE_PushButtonLabel, &option, &painter, this);
}

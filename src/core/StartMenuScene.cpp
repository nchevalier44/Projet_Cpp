#include "StartMenuScene.h"

StartMenuScene::StartMenuScene(QObject* parent) : QGraphicsScene(parent){


    //Ajouter la police Jersey10 (pixel art)
    int fontId = QFontDatabase::addApplicationFont("../assets/fonts/Jersey10-Regular.ttf");
    QString pixel_family = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont button_font(pixel_family, 40);
    QFont title_font(pixel_family, 65);


    //Ajouter le fond d'écran
    this->background.load("../assets/images/menu/background_start_menu.png");
    this->setSceneRect(0, 0, background.width(), background.height());


    //Ajouter les bouttons
    buttonsContainer = new QWidget();
    QVBoxLayout* buttonsLayout = new QVBoxLayout(buttonsContainer);

    QPushButton* startButton = new QPushButton("Start");
    QPushButton* optionsButton = new QPushButton("Options");
    QPushButton* exitButton = new QPushButton("Exit");

    startButton->setFont(button_font);
    optionsButton->setFont(button_font);
    exitButton->setFont(button_font);

    QObject::connect(startButton, &QPushButton::clicked, this, &StartMenuScene::startGameRequested);
    //A FAIRE : clique sur options
    QObject::connect(exitButton, &QPushButton::clicked, this, &QApplication::quit);

    buttonsLayout->addWidget(startButton);
    buttonsLayout->addWidget(optionsButton);
    buttonsLayout->addWidget(exitButton);

    buttonsContainer->setLayout(buttonsLayout);

    buttonsContainer->setAttribute(Qt::WA_OpaquePaintEvent); //Rendre le fond du widget transparent
    buttonsLayout->setSpacing(50); //Espacer les bouttons

    QGraphicsProxyWidget* proxyButtonsContainer = this->addWidget(buttonsContainer);
    qreal posX_buttons = (this->width() - buttonsContainer->width()) / 2;
    qreal posY_buttons = (this->height() - buttonsContainer->height()) / 2 + 100;
    proxyButtonsContainer->setPos(posX_buttons, posY_buttons);


    //Ajouter le titre
    QLabel* titleLabel = new QLabel("Title of the game");
    titleLabel->setFont(title_font);
    titleLabel->setAttribute(Qt::WA_OpaquePaintEvent);
    QGraphicsProxyWidget* proxyTitleContainer = this->addWidget(titleLabel);

    qreal posX_title = (this->width() - titleLabel->width()) / 2;
    qreal posY_title = (this->width() - titleLabel->height()) / 2 - 600;
    proxyTitleContainer->setPos(posX_title, posY_title);

}

void StartMenuScene::drawBackground(QPainter *painter, const QRectF &rect) {
    Q_UNUSED(rect);
    painter->drawPixmap(QPointF(0,0), background, sceneRect());
}
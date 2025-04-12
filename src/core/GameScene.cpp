#include "GameScene.h"


enum Direction {None, Up, Down, Left, Right};
Direction currentDirection = None;

GameScene::GameScene(QObject* parent) : QGraphicsScene(parent){
    //Setting up the scene
    this->background.load("../assets/images/menu/background_start_menu.png");
    this->setSceneRect(0, 0, background.width(), background.height());

    //Setting up the player's character
    this->character = new Player("Character", 3);
    this->character->setPos(400, 300);
    this->character->setSpeed(10);
    this->character->setScale(0.5);
    this->addItem(character);
    this->character->setMainView(mainView);

    //Setting up the UI
    HPWidget* hpWidget = new HPWidget(character->getMaxHp());
    hpWidget->setAttribute(Qt::WA_OpaquePaintEvent);

    hpWidget->move(10, 10);
    proxyWidget = addWidget(hpWidget);
    proxyWidget->setPos(10, 10);
    proxyWidget->setZValue(100); // Set the z-value to ensure it appears above other items
    proxyWidget->setAttribute(Qt::WA_OpaquePaintEvent); //get background of the widget transparent



    //Starting the timer to update the animation and mouvement
    this->timer = new QTimer(this);
    connect(this->timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
    this->timer->start(30); //every 30 milliseconds
}

//Mouvement fonction
//Adapt the animation according to the direction
void GameScene::keyPressEvent(QKeyEvent* event){
    if(event->isAutoRepeat()){
        return; //They key stay pressed so the walk animation can continue
    }
    activeKeys.append(event->key());
}


//Set the idle animation according to the last key pressed
void GameScene::keyReleaseEvent(QKeyEvent *event) {
    if(event->isAutoRepeat()){
        return; //They key stay pressed so the walk animation can continue
    }

    activeKeys.removeAll(event->key());

    if(!activeKeys.isEmpty()){
        return; //We change the animation to idle only if there are not others key pressed
    }
    switch(event->key()){
        case Qt::Key_Up:
        case Qt::Key_Z:
            character->backIdleAnimation();
            break;
        case Qt::Key_Down:
        case Qt::Key_S:
            character->frontIdleAnimation();
            break;
        case Qt::Key_Left:
        case Qt::Key_Q:
            character->leftIdleAnimation();
            break;
        case Qt::Key_Right:
        case Qt::Key_D:
            character->rightIdleAnimation();
            break;
    }
}



void GameScene::timerUpdate(){

    QPointF pos = character->pos();
    qreal posX = pos.rx();
    qreal posY = pos.ry();

    int n = activeKeys.length();

    //Check the last key pressed (currently always pressed)
    if(n==0){
        currentDirection = None;
    }
    else{
        int lastKey = activeKeys[n-1];
        if(lastKey == Qt::Key_Up || lastKey == Qt::Key_Z){
            if(currentDirection != Up){
                character->backWalkAnimation();
                currentDirection = Up;
            }
            posY -= character->getSpeed();
        }
        else if(lastKey == Qt::Key_Down || lastKey == Qt::Key_S){
            if(currentDirection != Down){
                character->frontWalkAnimation();
                currentDirection = Down;
            }
            posY += character->getSpeed();
        }
        else if(lastKey == Qt::Key_Left || lastKey == Qt::Key_Q){
            if(currentDirection != Left){
                character->leftWalkAnimation();
                currentDirection = Left;
            }
            posX -= character->getSpeed();
        }
        else if(lastKey == Qt::Key_Right || lastKey == Qt::Key_D){
            if(currentDirection != Right){
                character->rightWalkAnimation();
                currentDirection = Right;
            }
            posX += character->getSpeed();
        }
    }


    character->setPos(posX, posY);
    mainView->centerOn(character);

    QPointF cameraPos = views().first()->mapToScene(0, 0);
    proxyWidget->setPos(cameraPos.x() + 10, cameraPos.y() + 10);
}

GameScene::~GameScene(){

}

void GameScene::drawBackground(QPainter* painter, const QRectF& rect) {
    Q_UNUSED(rect);
    painter->drawPixmap(QPointF(0,0), background, sceneRect());
}
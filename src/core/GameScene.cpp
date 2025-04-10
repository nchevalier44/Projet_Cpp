#include "GameScene.h"

enum Direction {None, Up, Down, Left, Right};
Direction currentDirection = None;

GameScene::GameScene(QObject* parent) : QGraphicsScene(parent){



    this->character = new Entity("Character", 100);
    this->character->setPos(400, 300);
    this->addItem(this->character);

    this->timer = new QTimer(this);
    connect(this->timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
    this->timer->start(30); //toutes les 30 millisecondes
}

//Mouvement fonction
//Adapt the animation according to the direction
void GameScene::keyPressEvent(QKeyEvent* event){
    activeKeys.insert(event->key());
}


//Set the idle animation according to the last key pressed
void GameScene::keyReleaseEvent(QKeyEvent *event) {
    activeKeys.remove(event->key());
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

    //Check if the key is pressed
    if(activeKeys.contains(Qt::Key_Up) || activeKeys.contains(Qt::Key_Z)){
        if(currentDirection != Up){
            character->backWalkAnimation();
            currentDirection = Up;
        }
        posY -= 1;
    }
    if(activeKeys.contains(Qt::Key_Down) || activeKeys.contains(Qt::Key_S)){
        if(currentDirection != Down){
            character->frontWalkAnimation();
            currentDirection = Down;
        }
        posY += 1;
    }
    if(activeKeys.contains(Qt::Key_Left) || activeKeys.contains(Qt::Key_Q)){
        if(currentDirection != Left){
            character->leftWalkAnimation();
            currentDirection = Left;
        }
        posX -= 1;
    }
    if(activeKeys.contains(Qt::Key_Right) || activeKeys.contains(Qt::Key_D)){
        if(currentDirection != Right){
            character->rightWalkAnimation();
            currentDirection = Right;
        }
        posX += 1;
    }
    else {
        currentDirection = None;
    }
    character->setPos(posX, posY);
}

GameScene::~GameScene(){

}
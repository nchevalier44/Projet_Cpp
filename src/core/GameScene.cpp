#include "GameScene.h"

enum Direction {None, Up, Down, Left, Right};
Direction currentDirection = None;

GameScene::GameScene(QObject* parent) : QGraphicsScene(parent){

    this->character = new Entity("Character", 100);
    this->character->setPos(400, 300);
    this->addItem(character);

    this->timer = new QTimer(this);
    connect(this->timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
    this->timer->start(30); //toutes les 30 millisecondes
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
        int last_key = activeKeys[n-1];
        if(last_key == Qt::Key_Up || last_key == Qt::Key_Z){
            if(currentDirection != Up){
                character->backWalkAnimation();
                currentDirection = Up;
            }
            posY -= 1;
        }
        else if(last_key == Qt::Key_Down || last_key == Qt::Key_S){
            if(currentDirection != Down){
                character->frontWalkAnimation();
                currentDirection = Down;
            }
            posY += 1;
        }
        else if(last_key == Qt::Key_Left || last_key == Qt::Key_Q){
            if(currentDirection != Left){
                character->leftWalkAnimation();
                currentDirection = Left;
            }
            posX -= 1;
        }
        else if(last_key == Qt::Key_Right || last_key == Qt::Key_D){
            if(currentDirection != Right){
                character->rightWalkAnimation();
                currentDirection = Right;
            }
            posX += 1;
        }
    }


    character->setPos(posX, posY);
    mainView->centerOn(character);
}

GameScene::~GameScene(){

}
#include "MyScene.h"

#include <QPointF>

MyScene::MyScene(QObject* parent) : QGraphicsScene(parent){
    character = new CharacterItem("../assets/images/characters/test-char.png");
    this->addItem(character);

    this->timer = new QTimer(this);
    connect(this->timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
    this->timer->start(30); //toutes les 30 millisecondes
}

void MyScene::keyPressEvent(QKeyEvent* event){

    QPointF pos = character->pos();
    qreal posX = pos.rx();
    qreal posY = pos.ry();

    switch(event->key()){

        case Qt::Key_Up:
        case Qt::Key_Z:
            posY -= 1;
            break;

        case Qt::Key_Down:
        case Qt::Key_S:
            posY += 1;
            break;

        case Qt::Key_Left:
        case Qt::Key_Q:
            posX -= 1;
            break;

        case Qt::Key_Right:
        case Qt::Key_D:
            posX += 1;
            break;

    }
    character->setPos(posX, posY);

}
void MyScene::timerUpdate(){}

MyScene::~MyScene(){

}
#include "MyScene.h"

#include <QPointF>

MyScene::MyScene(QObject* parent) : QGraphicsScene(parent){
    character = new CharacterItem("../test-char.png");
    this->addItem(character);

    this->timer = new QTimer(this);
    connect(this->timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
    this->timer->start(30); //toutes les 30 millisecondes
}

void MyScene::keyPressEvent(QKeyEvent* event){
    QPointF pos = character->pos();
    switch(event->key()){
        case Qt::Key_Z:
            character->setPos(pos.rx(), pos.ry()-1);
            break;
        case Qt::Key_S:
            character->setPos(pos.rx(), pos.ry()+1);
            break;
        case Qt::Key_Q:
            character->setPos(pos.rx()-1, pos.ry());
            break;
        case Qt::Key_D:
            character->setPos(pos.rx()+1, pos.ry());
            break;

    }

}
void MyScene::timerUpdate(){}

MyScene::~MyScene(){

}
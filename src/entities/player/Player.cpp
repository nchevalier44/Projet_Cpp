#include "Player.h"
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QPainter>
#include <QDebug>
#include <QTimer>
#include <QVBoxLayout>


Player::Player(std::string name, int life) : Entity(name, life) {
    this->maxHp = life;
    setAnimation("../assets/images/characters/Front_idle.png", 8, 100);

}




///HEART UI

HPWidget::HPWidget(int maxLife, QWidget *parent) : QWidget(parent), maxLife(maxLife) {
    QHBoxLayout* lifeLayout = new QHBoxLayout(this);
    lifeLayout->setContentsMargins(10, 10, 10, 10);
    lifeLayout->setSpacing(5);


    for(int i = 0 ; i < maxLife ; i++){
        if(i < maxLife){
            QMovie* heart = new QMovie("../assets/images/characters/Fire_head.gif");
            QLabel* label = new QLabel();

            label->setMovie(heart);
            heart->start();
            label->setAttribute(Qt::WA_OpaquePaintEvent);

            lifeLayout->addWidget(label);
        }

    }
    this->setLayout(lifeLayout);

}

void HPWidget::setLife(int hp) {
    for(int i = 0 ; i < maxLife ; i++){
        if(i < hp){
            life[i]->setFileName("../assets/images/characters/Fire_head.gif");
        }
        else{
            life[i]->setFileName("../assets/images/characters/Fire_head.gif");
        }
    }
}

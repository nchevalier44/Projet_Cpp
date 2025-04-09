#include "Entity.h"
#include <iostream>

Entity::Entity(QString spriteSheet, int frameCount, int animationSpeed, std::string name, int hp) : hp(hp), name(name) {
    this->spriteSheet = new QPixmap(spriteSheet);
    this->currentFrame = 0;
    this->frameCount = frameCount;
    this->frameWidth = this->spriteSheet->width() / frameCount;
    this->frameHeight = this->spriteSheet->height();
    this->animationSpeed = animationSpeed;
    this->timer = new QTimer();

    connect(this->timer, &QTimer::timeout, this, &Entity::updateAnimation);
    this->timer->start(animationSpeed);
}

QRectF Entity::boundingRect() const {
    return QRectF(0, 0, frameWidth, frameHeight);
}

void Entity::updateAnimation() {
    // Update the current frame
    currentFrame = (currentFrame + 1) % frameCount;


    update();
}



void Entity::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {


    // Draw the current frame of the sprite sheet
    QRect sourceRect(currentFrame * frameWidth, 0, frameWidth, frameHeight);
    painter->drawPixmap(0, 0, frameWidth, frameHeight, *spriteSheet, sourceRect.x(), sourceRect.y(), sourceRect.width(), sourceRect.height());

    Q_UNUSED(option);
    Q_UNUSED(widget);
}



#include "Entity.h"
#include <iostream>

Entity::Entity(std::string name, int hp) : hp(hp), name(name) , spriteSheet(nullptr), currentFrame(0), frameCount(0), frameWidth(0), frameHeight(0), animationSpeed(100), timer(nullptr) {
    frontIdleAnimation();
}

void Entity::stopAnimation(){
    if (timer) {
        timer->stop();
    }
    delete timer;
    timer = nullptr;
}

void Entity::setAnimation(QString newSpriteSheet, int newFrameCount, int newAnimationSpeed){
    if(newSpriteSheet == currentSpriteSheetPath) {
        return; // No change in sprite sheet
    }

    // Clean up the old sprite sheet if it exists
    if (spriteSheet) {
        delete spriteSheet;
        spriteSheet = nullptr;
    }
    stopAnimation();

    // Update the current sprite sheet path
    currentSpriteSheetPath = newSpriteSheet;
    // Load the new sprite sheet
    this->spriteSheet = new QPixmap(newSpriteSheet);
    this->currentFrame = 0;
    this->frameCount = newFrameCount;
    this->frameWidth = this->spriteSheet->width() / frameCount;
    this->frameHeight = this->spriteSheet->height();
    this->animationSpeed = newAnimationSpeed;
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



#include "Entity.h"
#include <iostream>
#include <QThread>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QWidget>
#include <QGraphicsScene>
#include <QTimer>

#include "../core/GameScene.h"

Entity::Entity(std::string name, int hp, GameScene* scene, QGraphicsItem* parent) : hp(hp), name(name), gameScene(scene), QGraphicsObject(parent) {
}

Entity::~Entity() {
    if (spriteSheet) {
        delete spriteSheet;
        spriteSheet = nullptr;
    }
}

QPointF Entity::getCenterPosition() const {
    QPointF centerOffset(sceneBoundingRect().width() / 2, sceneBoundingRect().height() / 2);
    return this->pos() + centerOffset;
}



void Entity::horizontalFlip(){
    //We flip the pixmap to change the direction of the animation
    QPixmap* currentPixmap = this->getSpriteSheet();
    QPixmap* flippedPixmap = new QPixmap(currentPixmap->transformed(QTransform().scale(-1, 1)));
    this->setSpriteSheet(flippedPixmap);

    //We delete the old pixmap in order to avoid creating many pixmap we don't use
    if(currentPixmap != nullptr) delete currentPixmap;
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
        return; // No change in the sprite sheet
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
    this->timer = new QTimer(this);

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
    painter->drawRect(boundingRect()); // Optional: Draw the bounding rect for debugging

    painter->setPen(QPen(Qt::green, 2, Qt::DashLine));
    painter->drawPath(shape());

    Q_UNUSED(option);
    Q_UNUSED(widget);
    //Draw the shape
}

void Entity::attackEntity(Entity* entity) {
    attacking = true;
    this->attackAnimation();
    entity->takeDamage(this->getDamage());

}

void Entity::takeDamage(int d) {
    if(hp - d <= 0) {
        hp = 0;
        deathAnimation();
    } else {
        hp -= d;
    }
}

void Entity::moveEntity(qreal posX, qreal posY){
    if(hp == 0) return; //if dead -> don't move

    Direction direction = this->getCurrentDirection();
    qreal posEntityX = this->getCenterPosition().x();
    qreal posEntityY = this->getCenterPosition().y();
    qreal dx = posX - posEntityX;
    qreal dy = posY - posEntityY;
    if(dx < 0){
        posEntityX -= speed;
        this->setCurrentDirection(Left);
    } else if(dx > 0){
        posEntityX += speed;
        this->setCurrentDirection(Right);
    }
    if(dy < 0){
        posEntityY -= speed;
    } else if(dy > 0){
        posEntityY += speed;
    }

    if(direction != currentDirection){
        horizontalFlipped = !horizontalFlipped;
        this->horizontalFlip();
    }

    float distance = sqrt(pow(posX - posEntityX, 2) + pow(posY - posEntityY, 2));
    if(distance >= rangeAttack && !(attacking)){
        this->setCenterPosition(QPointF(posEntityX, posEntityY));
    }
}


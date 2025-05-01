#include "Entity.h"
#include <iostream>
#include <QThread>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QWidget>
#include <QGraphicsScene>
//////////PROJECTILE MEHTODS//////////

Projectile::Projectile(int damage, int speed, int distanceMax, QString path, QPointF pos, QPointF direction, QGraphicsObject* parent=nullptr)
    : speed(speed), damage(damage), distanceMax(distanceMax), distanceTravelled(0), QGraphicsObject(parent){
    this->setPos(pos);
    this->rotationAngle = std::atan2(direction.y(), direction.x())*180/M_PI;
    this->dx = std::cos(rotationAngle * M_PI / 180);
    this->dy = std::sin(rotationAngle * M_PI / 180);
    this->movie = new QMovie(this);
    movie->setFileName(path);
    this->timer = new QTimer(this);
}



void Projectile::move(){
    //Adding a small moving to the projectile
    this->setX(this->x() + dx*speed);
    this->setY(this->y() + dy*speed);
    distanceTravelled += speed;
    if(distanceTravelled >= distanceMax){
        setEndAnimation("",0,0);
        dx = 0;
        dy = 0;
        distanceTravelled = 0;
    }
}

void Projectile::deleteProjectile() {
    if (movie) {
        movie->stop();
        delete movie;
        movie = nullptr;
    }
    if (timer) {
        timer->stop();
        delete timer;
        timer = nullptr;
    }
    delete this;
}






///////////////ENTITY METHODS///////////////
Entity::Entity(std::string name, int hp, QGraphicsItem* parent) : hp(hp), name(name), QGraphicsObject(parent) {
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

void Entity::setCenterPosition(QPointF newPos) {
    setPos(newPos - boundingRect().center());
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

void Entity::takeDamage(int damage) {
    qDebug() << "Take damage1";
    if(hp - damage <= 0) {
        hp = 0;
        deathAnimation();
    } else {
        this->hp -= damage;
    }
}



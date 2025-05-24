#include "Entity.h"
#include <iostream>
#include <QThread>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QWidget>
#include <QGraphicsScene>
#include <QTimer>
#include "Projectile.h"
#include "../core/GameScene.h"

Entity::Entity(std::string name, int hp, ScoreManager* scoreManager, GameScene* scene, QGraphicsItem* parent) : hp(hp), name(name), scoreManager(scoreManager), gameScene(scene), QGraphicsObject(parent) {
    this->setZValue(40);
}

Entity::~Entity() {
    if (spriteSheet) {
        delete spriteSheet;
        spriteSheet = nullptr;
        delete timer;
        timer = nullptr;
    }
}

QPointF Entity::getCenterPosition() const {
    return mapToScene(shape().boundingRect().center());
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
        gameScene->getTimerList().removeAll(timer);
        delete timer;
        timer = nullptr;
    }

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

    if(spriteSheet->isNull()) {
        qDebug() << "Error: Could not load sprite sheet " << newSpriteSheet;
        return;
    }
    this->currentFrame = 0;
    this->frameCount = newFrameCount;
    this->frameWidth = this->spriteSheet->width() / frameCount;
    this->frameHeight = this->spriteSheet->height();
    this->animationSpeed = newAnimationSpeed;
    this->timer = new QTimer(this);
    gameScene->getTimerList().append(timer);

    connect(this->timer, &QTimer::timeout, this, &Entity::updateAnimation);
    this->timer->start(animationSpeed);
    if(this->isHorizontalFlipped() && newSpriteSheet != PATH_PLAYER_DEATH) this->horizontalFlip();
    if(gameScene->isGamePaused()){
        timer->stop();
    }
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
}

void Entity::attackEntity(Entity* entity) {
    attacking = true;
    this->attackAnimation();
    entity->takeDamage(this->getDamage(), this);
}

void Entity::takeDamage(int d, Entity* attacker, Projectile* projectile) {
    if(isDead) return;

    if(projectile){
        this->takeKnockback(projectile->getCenterPosition().x(), projectile->getCenterPosition().y());
    }else if(attacker){
        this->takeKnockback(attacker->getCenterPosition().x(), attacker->getCenterPosition().y());
    }

    hitSound();

    //Display a red screen to indicate damage
    QGraphicsColorizeEffect* effect = new QGraphicsColorizeEffect(this);
    effect->setColor(Qt::darkRed);
    effect->setStrength(0.6);
    this->setGraphicsEffect(effect);

    QTimer::singleShot(150, [this]() {
        this->setGraphicsEffect(nullptr);
    });

    if(hp - d <= 0) {
        hp = 0;
        isDead = true;
        deathAnimation();
        deathSound();
        Player* player = dynamic_cast<Player*>(attacker); //return nullptr if attacker is not a player
        if (player) { //if attacker is a player
            scoreManager->getActualScore()->setScore(scoreManager->getActualScore()->getScore() + this->getScore());
        }
    } else {
        hp -= d;
    }
}

void Entity::takeKnockback(int originX, int originY){
    isTakingKnockback = true;
    QTimer* knockbackTimer = new QTimer();
    knockbackTimer->setInterval(30);

    connect(knockbackTimer, &QTimer::timeout, [this, originX, originY]() {
        qreal posEntityX = this->getCenterPosition().x();
        qreal posEntityY = this->getCenterPosition().y();

        qreal finalX = posEntityX;
        qreal finalY = posEntityY;

        if(originX - posEntityX < this->sceneBoundingRect().width() * 0.05){
            finalX += originX;
        } else if(originX + posEntityX > this->sceneBoundingRect().width() * 0.05){
            finalX -= originX;
        }

        if(originY - posEntityY < this->sceneBoundingRect().height() * 0.05){
            finalY += originY;
        } else if(originY + posEntityY > this->sceneBoundingRect().height() * 0.05){
            finalY -= originY;
        }

        this->moveEntity(finalX, finalY, true);
    });

    knockbackTimer->start();
    QTimer::singleShot(200, this, [knockbackTimer, this](){
        knockbackTimer->stop();
        delete knockbackTimer;
        this->isTakingKnockback = false;
    });
}


void Entity::moveEntity(qreal posX, qreal posY, bool forceMove){
    if(hp == 0) return; //if dead -> don't move

    Direction direction = this->getCurrentDirection();
    qreal posEntityX = this->getCenterPosition().x();
    qreal posEntityY = this->getCenterPosition().y();

    qreal dx = 0;
    qreal dy = 0;

    int distanceX = posX - posEntityX;
    int distanceY = posY - posEntityY;

    if(distanceX < 0){
        dx = -speed;
        this->setCurrentDirection(Left);
    } else if(distanceX > 0){
        dx = speed;
        this->setCurrentDirection(Right);
    }

    if(distanceY < 0){
        dy = -speed;
    } else if(distanceY > 0){
        dy = speed;
    }

    //If the speed is bigger than the distance to travel (in x or y), I set delta to the distance
    // else the Entity do right and left in continue because the point he want to go is between +speed and -speed
    if(abs(distanceX) < speed){
        if(speed < 0){
            dx = -distanceX;
        } else{
            dx = distanceX;
        }
    }
    if(abs(distanceY) < speed){
        if(speed < 0){
            dy = -distanceY;
        } else{
            dy = distanceY;
        }
    }

    if(!isBeenTakingKnockback() && direction != currentDirection){
        horizontalFlipped = !horizontalFlipped;
        this->horizontalFlip();
    }

    float distance = sqrt(pow(posX - posEntityX, 2) + pow(posY - posEntityY, 2));
    if(forceMove || (distance >= rangeAttack && !(attacking))){
        moveEntityCollision(dx, dy);
        this->moveAnimation();
    }
}

void Entity::moveEntityCollision(qreal dx, qreal dy){

    QList<QGraphicsItem*> collisions;
    int numberCollisions;
    int i;

    //Check if there is horizontal collision with an object
    this->moveBy(dx, 0);
    collisions = this->collidingItems();
    numberCollisions = collisions.count();
    i = 0;

    //Skip all collisions which don't interest us
    while(i < numberCollisions && collisions[i]->data(0).toString() != "collision"){
        i++;
    }
    if(i != numberCollisions){
        this->moveBy(-dx, 0);
    }

    //Check if there is vertical collision with an object
    this->moveBy(0, dy);
    collisions = this->collidingItems();
    numberCollisions = collisions.count();
    i = 0;

    //Skip all collisions which don't interest us
    while(i < numberCollisions && collisions[i]->data(0).toString() != "collision"){
        i++;
    }
    if(i != numberCollisions){
        this->moveBy(0, -dy);
    }
}


void Entity::deathSound() {
    QSoundEffect* deathSFX = new QSoundEffect();
    connect(deathSFX, &QSoundEffect::loadedChanged, deathSFX, &QSoundEffect::play);
    deathSFX->setSource(QUrl::fromLocalFile(pathDeathSound));
    deathSFX->setVolume(0.4);
    gameScene->getAudioManager()->addSFXObject(deathSFX, deathSFX->volume());
    connect(deathSFX, &QSoundEffect::playingChanged, [deathSFX](){
        if(!deathSFX->isPlaying()){
            delete deathSFX;
        }
    });
}

void Entity::hitSound(){
    QSoundEffect* hitSFX = new QSoundEffect();
    connect(hitSFX, &QSoundEffect::loadedChanged, hitSFX, &QSoundEffect::play);
    hitSFX->setSource(QUrl::fromLocalFile(pathHitSound));
    hitSFX->setVolume(0.4);
    gameScene->getAudioManager()->addSFXObject(hitSFX, hitSFX->volume());
    connect(hitSFX, &QSoundEffect::playingChanged, [hitSFX](){
        if(!hitSFX->isPlaying()){
            delete hitSFX;
        }
    });
}

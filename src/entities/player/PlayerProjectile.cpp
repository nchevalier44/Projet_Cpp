#include <QtGui>
#include "PlayerProjectile.h"
#include "../../core/GameScene.h"

PlayerProjectile::PlayerProjectile(int damage, int speed, int distanceMax, QString spriteSheet, QPointF pos, QPointF direction, GameScene* scene, Entity* proprietary, QGraphicsObject* parent)
        : Projectile(damage, speed, distanceMax, spriteSheet, pos, direction, scene, proprietary, parent) {
    frameWidth = 32;
    frameHeight = 21;
    QPointF centerOffset(frameWidth / 2, frameHeight / 2);
    this->setPos(pos - centerOffset);
    throwProjectile();
}

void PlayerProjectile::throwProjectile() {
    setStartAnimation(PATH_PLAYER_PROJECTILE_GROW, 6, 100);
}

void PlayerProjectile::setStartAnimation(QString spriteSheet, int frameCount, int animationSpeed) {
    if(movie){
        movie->stop();
        delete movie;
        movie = nullptr;
    }
    this->movie = new QMovie(spriteSheet);
    this->movie->start();

    //Add a singleshot timer
    QTimer::singleShot(frameCount*animationSpeed, this, &PlayerProjectile::startMove);
}

void PlayerProjectile::setMiddleAnimation(QString spriteSheet, int frameCount, int animationSpeed) {
    if(movie){
        movie->stop();
        delete movie;
        movie = nullptr;
    }
    this->movie = new QMovie(spriteSheet);
    this->movie->start();
}

void PlayerProjectile::setEndAnimation(QString spriteSheet, int frameCount, int animationSpeed) {
    if(movie){
        movie->stop();
        delete movie;
        movie = nullptr;
    }
    this->movie = new QMovie(PATH_PLAYER_PROJECTILE_FADE);

    timerEndMovie = new QTimer();
    connect(timerEndMovie, &QTimer::timeout, this, [this](){
        if(this->movie->currentFrameNumber() == this->movie->frameCount() - 1){
            this->movie->stop();
            this->gameScene->removeProjectile(this);
            timerEndMovie->stop();
            delete timerEndMovie;
            delete movie;
            movie = nullptr;
            timerEndMovie = nullptr;
        } else{
            this->movie->jumpToNextFrame();
            this->gameScene->update(this->sceneBoundingRect());
            timerEndMovie->setInterval(this->movie->nextFrameDelay());
        }
    });
    timerEndMovie->start();
}

void PlayerProjectile::startMove() {
    setMiddleAnimation(PATH_PLAYER_PROJECTILE);
    //Starting the moving

    gameScene->addProjectile(this);
}
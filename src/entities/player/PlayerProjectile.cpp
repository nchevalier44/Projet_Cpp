#include <QtGui>
#include "PlayerProjectile.h"
#include "../../core/GameScene.h"

PlayerProjectile::PlayerProjectile(int damage, int speed, int distanceMax, QString spriteSheet, QPointF pos, QPointF direction, GameScene* scene, Entity* proprietary, QGraphicsObject* parent)
        : Projectile(damage, speed, distanceMax, spriteSheet, pos, direction, scene, proprietary, parent) {
    frameWidth = 32;
    frameHeight = 21;
    throwProjectile();
    this->setTransform(QTransform::fromScale(2, 2));
    pathMissileMoveSound = PATH_PLAYER_PROJECTILE_MOVE_SOUND;
}

void PlayerProjectile::throwProjectile() {
    setStartAnimation(PATH_PLAYER_PROJECTILE_GROW);
    QTimer::singleShot(6*100, this, &PlayerProjectile::startMove);
}

void PlayerProjectile::setEndAnimation(QString spriteSheet, int frameCount, int animationSpeed) {
    if(movie){
        movie->stop();
        gameScene->getMovieList().removeAll(movie);
        delete movie;
        movie = nullptr;
    }
    this->movie = new QMovie(PATH_PLAYER_PROJECTILE_FADE);
    gameScene->getMovieList().append(movie);

    timerEndMovie = new QTimer(this);
    gameScene->getTimerList().append(timerEndMovie);
    connect(timerEndMovie, &QTimer::timeout, this, [this](){
        if(this->movie->currentFrameNumber() == this->movie->frameCount() - 1){
            movie->stop();
            timerEndMovie->stop();
            gameScene->getMovieList().removeAll(movie);
            gameScene->getTimerList().removeAll(timerEndMovie);
            delete movie;
            movie = nullptr;
            timerEndMovie = nullptr;
            gameScene->removeProjectile(this);
        } else{
            movie->jumpToNextFrame();
            gameScene->update(this->sceneBoundingRect());
            timerEndMovie->setInterval(this->movie->nextFrameDelay());
        }
    });
    timerEndMovie->start();
    if(gameScene->isGamePaused()){
        timerEndMovie->stop();
    }
}

void PlayerProjectile::startMove() {
    setMiddleAnimation(PATH_PLAYER_PROJECTILE);
    //Starting the moving
    gameScene->addProjectile(this);
    missileMoveSound();
}
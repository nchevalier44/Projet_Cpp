#include "GoblinProjectile.h"
#include "../../core/GameScene.h"

GoblinProjectile::GoblinProjectile(int damage, int speed, int distanceMax, QString spriteSheet, QPointF pos, QPointF direction, GameScene* scene, Entity* proprietary, QGraphicsObject* parent)
        : Projectile(damage, speed, distanceMax, spriteSheet, pos, direction, scene, proprietary, parent) {
    frameWidth = 32;
    frameHeight = 21;
    QPointF centerOffset(frameWidth / 2, frameHeight / 2);
    this->setPos(pos - centerOffset);
    this->setTransform(QTransform::fromScale(2, 2));
    pathMissileMoveSound = PATH_GOBLIN_PROJECTILE_MOVE_SOUND;
    throwProjectile();
}

void GoblinProjectile::throwProjectile() {
    setMiddleAnimation(PATH_GOBLIN_PROJECTILE_MOVE);
    startMove();
}

void GoblinProjectile::setEndAnimation(QString spriteSheet, int frameCount, int animationSpeed) {
    if(movie){ //clear previous movie
        movie->stop();
        gameScene->getMovieList().removeAll(movie);
        delete movie;
        movie = nullptr;
    }
    this->movie = new QMovie(PATH_GOBLIN_PROJECTILE_BLOW);
    gameScene->getMovieList().append(movie);

    timerEndMovie = new QTimer();
    gameScene->getTimerList().append(timerEndMovie);
    connect(timerEndMovie, &QTimer::timeout, this, [this](){
        if(this->movie->currentFrameNumber() == this->movie->frameCount() - 1){
            this->movie->stop();
            timerEndMovie->stop();
            gameScene->getMovieList().removeAll(movie);
            gameScene->getTimerList().removeAll(timerEndMovie);
            delete timerEndMovie;
            delete movie;
            movie = nullptr;
            timerEndMovie = nullptr;
            this->gameScene->removeProjectile(this);
        } else{
            this->movie->jumpToNextFrame();
            this->gameScene->update(this->sceneBoundingRect());
            timerEndMovie->setInterval(this->movie->nextFrameDelay());
        }
    });
    timerEndMovie->start();

    if(gameScene->isGamePaused()){
        timerEndMovie->stop();
    }
}

void GoblinProjectile::startMove() {
    setMiddleAnimation(PATH_GOBLIN_PROJECTILE_MOVE);
    //Starting the moving
    gameScene->addProjectile(this);
    missileMoveSound();
}
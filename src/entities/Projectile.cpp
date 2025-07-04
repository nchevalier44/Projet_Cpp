#include "Projectile.h"
#include "../core/GameScene.h"
#include "player/PlayerShield.h"
#include "Entity.h"

Projectile::Projectile(int damage, int speed, int distanceMax, QString path, QPointF pos, QPointF direction, GameScene* scene, Entity* proprietary, QGraphicsObject* parent)
        : proprietary(proprietary), gameScene(scene), speed(speed), damage(damage), distanceMax(distanceMax), distanceTravelled(0), QGraphicsObject(parent){
    this->setPos(pos);
    QPointF centerOffset(sceneBoundingRect().width() / 2, sceneBoundingRect().height() / 2);
    QPointF normalizedDirection = direction - centerOffset;

    this->rotationAngle = std::atan2(normalizedDirection.y(), normalizedDirection.x()) * 180 / M_PI;
    this->dx = std::cos(rotationAngle * M_PI / 180);
    this->dy = std::sin(rotationAngle * M_PI / 180);
    this->movie = new QMovie(this);
    movie->setFileName(path);
    gameScene->getMovieList().append(movie);
}

Projectile::~Projectile(){
    if(missileMoveSFX){
        missileMoveSFX->stop();
        delete missileMoveSFX;
        missileMoveSFX = nullptr;
    }
}

QPointF Projectile::getCenterPosition() const {
    return mapToScene(shape().boundingRect().center());
}

void Projectile::startMove(){
    gameScene->addProjectile(this);
    missileMoveSound();
}

QRectF Projectile::boundingRect() const {
    return QRectF(0, 0, 40, 40);
}

QPainterPath Projectile::shape() const {
    QPainterPath path;
    // Circular shape for the projectile
    path.addEllipse(boundingRect().center(), 9, 9);  // rayon 8px
    return path;
}

void Projectile::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    // Display the current frame of the movie
    if (movie && !movie->currentPixmap().isNull()) {
        painter->save();
        painter->translate(boundingRect().center());
        painter->rotate(rotationAngle);
        painter->translate(-boundingRect().center());
        painter->drawPixmap(0, 10, frameWidth, frameHeight, movie->currentPixmap());
        painter->restore();
    }
}

void Projectile::moveProjectile(){
    if(isBeenDeleting) return;

    //Adding a small moving to the projectile
    this->setX(this->x() + dx*speed);
    this->setY(this->y() + dy*speed);

    //Check for collisions
    QList<QGraphicsItem*> collisions = gameScene->collidingItems(this);
    int n = collisions.length();
    int i = 0;
    bool hasCollided = false;
    while(!hasCollided && i < n){
        //dynamic_cast<Entity*> return an Entity if it's an entity else nullptr
        Entity* testEntity = dynamic_cast<Entity*>(collisions[i]);
        if(testEntity){
            if(proprietary){
                if(testEntity != proprietary){
                    testEntity->takeDamage(damage, this->proprietary);
                    hasCollided = true;
                }
            } else{
                testEntity->takeDamage(damage, this->proprietary);
                hasCollided = true;
            }
        }
        PlayerShield* testShield = dynamic_cast<PlayerShield*>(collisions[i]);
        if(testShield){
            if(testEntity != proprietary){
                if (dynamic_cast<Player*>(proprietary)) {
                    // Player shooted so we ignore the shield collision
                    ++i;
                    continue;
                }
                hasCollided = true;
                testShield->decreaseHP();
            }
        }
        if(collisions[i]->data(0) == "collision"){
            hasCollided = true;
        }
        i++;
    }

    //Check for max distance traveled
    distanceTravelled += speed;
    if(distanceTravelled >= distanceMax || hasCollided){
        missileBlowSound();
        setEndAnimation("",0,0);
        isBeenDeleting = true;
        if(missileMoveSFX){
            missileMoveSFX->stop();
            delete missileMoveSFX;
            missileMoveSFX = nullptr;
        }

    }
}

void Projectile::setStartAnimation(QString spriteSheet) {
    if(movie){
        movie->stop();
        gameScene->getMovieList().removeAll(movie);
        delete movie;
        movie = nullptr;
    }
    this->movie = new QMovie(spriteSheet);
    gameScene->getMovieList().append(movie);
    this->movie->start();
    if(gameScene->isGamePaused()){
        movie->setPaused(true);
    }
}

void Projectile::setMiddleAnimation(QString spriteSheet) {
    if(movie){
        movie->stop();
        gameScene->getMovieList().removeAll(movie);
        delete movie;
        movie = nullptr;
    }
    this->movie = new QMovie(spriteSheet);
    gameScene->getMovieList().append(movie);
    this->movie->start();
    if(gameScene->isGamePaused()){
        movie->setPaused(true);
    }
}

void Projectile::missileMoveSound(){
    // Load the sound effect for missile movement
    if(pathMissileMoveSound.isEmpty()) return;
    missileMoveSFX = new QSoundEffect();
    connect(missileMoveSFX, &QSoundEffect::loadedChanged, missileMoveSFX, &QSoundEffect::play);
    missileMoveSFX->setSource(QUrl::fromLocalFile(pathMissileMoveSound));
    missileMoveSFX->setVolume(0.25);
    gameScene->getAudioManager()->addSFXObject(missileMoveSFX, missileMoveSFX->volume());
    missileMoveSFX->setLoopCount(QSoundEffect::Infinite);
}

void Projectile::missileBlowSound(){
    // Load the sound effect for missile blow
    if(pathMissileBlowSound.isEmpty()) return;
    QSoundEffect* missileBlowSFX = new QSoundEffect();
    connect(missileBlowSFX, &QSoundEffect::loadedChanged, missileBlowSFX, &QSoundEffect::play);
    missileBlowSFX->setSource(QUrl::fromLocalFile(pathMissileBlowSound));
    missileBlowSFX->setVolume(0.25);
    gameScene->getAudioManager()->addSFXObject(missileBlowSFX, missileBlowSFX->volume());
    connect(missileBlowSFX, &QSoundEffect::playingChanged, [missileBlowSFX](){
        if(!missileBlowSFX->isPlaying()){
            delete missileBlowSFX;
        }
    });
}
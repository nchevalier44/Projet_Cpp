#include "Projectile.h"
#include "../core/GameScene.h"
#include "player/PlayerShield.h"


Projectile::Projectile(int damage, int speed, int distanceMax, QString path, QPointF pos, QPointF direction, GameScene* scene, Entity* proprietary, QGraphicsObject* parent)
        : proprietary(proprietary), gameScene(scene), speed(speed), damage(damage), distanceMax(distanceMax), distanceTravelled(0), QGraphicsObject(parent){
    this->setPos(pos);
    this->rotationAngle = std::atan2(direction.y(), direction.x())*180/M_PI;
    this->dx = std::cos(rotationAngle * M_PI / 180);
    this->dy = std::sin(rotationAngle * M_PI / 180);
    this->movie = new QMovie(this);
    movie->setFileName(path);
    gameScene->getMovieList().append(movie);
}

void Projectile::startMove(){
    gameScene->addProjectile(this);
}

QRectF Projectile::boundingRect() const {
    return QRectF(0, 0, 40, 40);
}

QPainterPath Projectile::shape() const {
    QPainterPath path;
    // Hitbox circulaire plus petite que l'image
    path.addEllipse(boundingRect().center(), 9, 9);  // rayon 8px
    return path;
}

void Projectile::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    // Affichage de l'image actuelle de l'animation
    if (movie && !movie->currentPixmap().isNull()) {
        painter->save();
        painter->translate(boundingRect().center());
        painter->rotate(rotationAngle);
        painter->translate(-boundingRect().center());
        painter->drawPixmap(0, 10, frameWidth, frameHeight, movie->currentPixmap());
        painter->restore();
    }

    // Debug : dessiner boundingRect (en rouge)
    painter->setPen(QPen(Qt::red, 1, Qt::DashLine));
    painter->drawRect(boundingRect());

    // Debug : dessiner shape (en bleu)
    painter->setPen(QPen(Qt::blue, 1));
    painter->drawPath(shape());

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
        Player* player = dynamic_cast<Player*>(collisions[i]);
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
                    // Le joueur a tiré -> on ignore le bouclier
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
        setEndAnimation("",0,0);
        isBeenDeleting = true;
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
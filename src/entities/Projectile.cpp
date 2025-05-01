#include "Projectile.h"
#include "../core/GameScene.h"


void Projectile::startMove(){
    gameScene->addProjectile(this);
}

QRectF Projectile::boundingRect() const {
    return QRectF(0, 0, 40, 40);
}

QPainterPath Projectile::shape() const {
    QPainterPath path;
    // Hitbox circulaire plus petite que l'image
    path.addEllipse(boundingRect().center(), 8, 8);  // rayon 8px
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

    /*
    // Debug : dessiner boundingRect (en rouge)
    painter->setPen(QPen(Qt::red, 1, Qt::DashLine));
    painter->drawRect(boundingRect());

    // Debug : dessiner shape (en bleu)
    painter->setPen(QPen(Qt::blue, 1));
    painter->drawPath(shape());
     */

}


Projectile::Projectile(int damage, int speed, int distanceMax, QString path, QPointF pos, QPointF direction, GameScene* scene, QGraphicsObject* parent=nullptr)
        : gameScene(scene), speed(speed), damage(damage), distanceMax(distanceMax), distanceTravelled(0), QGraphicsObject(parent){
    this->setPos(pos);
    this->rotationAngle = std::atan2(direction.y(), direction.x())*180/M_PI;
    this->dx = std::cos(rotationAngle * M_PI / 180);
    this->dy = std::sin(rotationAngle * M_PI / 180);
    this->movie = new QMovie(this);
    movie->setFileName(path);
}


void Projectile::moveProjectile(){
    //Adding a small moving to the projectile
    this->setX(this->x() + dx*speed);
    this->setY(this->y() + dy*speed);
    //QList<QGraphicsItem*> collisions = gameScene->collidingItems(this);

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
    delete this;
}
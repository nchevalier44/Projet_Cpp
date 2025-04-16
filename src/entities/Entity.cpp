#include "Entity.h"
#include <iostream>
//////////PROJECTILE MEHTODS//////////

Projectile::Projectile(int damage, int speed, int distanceMax, QString path, QPointF pos, QPointF direction)
    : speed(speed), damage(damage), distanceMax(distanceMax), distanceTravelled(0){
    frameWidth = 32;
    frameHeight = 21;

    QPointF centerOffset(frameWidth / 2, frameHeight / 2.2);
    this->setPos(pos - centerOffset);



    this->rotationAngle = std::atan2(direction.y(), direction.x())*180/M_PI;
    this->dx = std::cos(rotationAngle * M_PI / 180);
    this->dy = std::sin(rotationAngle * M_PI / 180);
    this->movie = new QMovie(path);
    this->timer = new QTimer();
    this->movie->start();


    //Starting the moving
    connect(this->timer, &QTimer::timeout, this, &Projectile::move);
    this->timer->start(10);
    //Verify if the movie is loaded
    if (!movie->isValid()) {
        qDebug() << "Error loading movie:" << path;
        delete this->movie;
        this->movie = nullptr;
        return;
    }


}


void Projectile::move(){
    this->setX(this->x() + dx*speed);
    this->setY(this->y() + dy*speed);
    distanceTravelled += speed;
    if(distanceTravelled >= distanceMax){
        this->movie->stop();
        delete this->movie;
        this->movie = nullptr;
        this->timer->stop();
        delete this->timer;
        this->timer = nullptr;

        delete this;
    }
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



///////////////ENTITY METHODS///////////////
Entity::Entity(std::string name, int hp) : hp(hp), name(name) , spriteSheet(nullptr), currentFrame(0), frameCount(0), frameWidth(0), frameHeight(0), animationSpeed(100), timer(nullptr) {
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
    painter->drawRect(boundingRect()); // Optional: Draw the bounding rect for debugging

    painter->setPen(QPen(Qt::green, 2, Qt::DashLine));
    painter->drawPath(shape());
    Q_UNUSED(option);
    Q_UNUSED(widget);
    //Draw the shape
}



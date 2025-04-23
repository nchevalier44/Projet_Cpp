#include "Player.h"
#include <QTimer>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QAbstractAnimation>
#include <QThread>



Player::Player(std::string name, int life) : Entity(name, life) {
    this->maxHp = life;
    setAnimation("../assets/images/characters/Front_idle.png", 8, 100);
    movingSound = new QSoundEffect(this);
    movingSound->setSource(QUrl::fromLocalFile("../assets/sounds_effects/footsteps.wav"));
    movingSound->setLoopCount(QSoundEffect::Infinite);
    movingSound->setVolume(30);
}


void Player::takeDamage(int damage) {
    if(isDead) return;
    setHp(hp - damage);
    if(hp <= 0){
        isDead = true;
        QTimer::singleShot(1000, mainView, &MainView::displayDeathScreen);
    }

    else{
        //Create a red screen to indicate damage
        QWidget* DamageScreen = new QWidget(Player::getMainView());
        DamageScreen->setStyleSheet("background-color: red;");
        DamageScreen->setGeometry(Player::getMainView()->rect());
        DamageScreen->show();

        //Fade of black background
        QGraphicsOpacityEffect* fadeBackgroundEffect = new QGraphicsOpacityEffect(DamageScreen);
        DamageScreen->setGraphicsEffect(fadeBackgroundEffect);
        QPropertyAnimation *animationBackground = new QPropertyAnimation(fadeBackgroundEffect, "opacity");
        animationBackground->setDuration(150);
        animationBackground->setStartValue(0);
        animationBackground->setEndValue(0.5);
        animationBackground->start(QAbstractAnimation::DeleteWhenStopped);
        QTimer::singleShot(150, [DamageScreen]() {
            DamageScreen->deleteLater();

        });

    }
}

Projectile* Player::shootProjectile(QPointF target, QGraphicsScene* scene) {
    //Ajust the position of the projectile
    QPointF posInit = this->pos();
    posInit.setX(posInit.x() + frameWidth/20);
    posInit.setY(posInit.y() + frameHeight/20);
    QPointF direction = target - posInit;

    //Adjusting the position of the projectile to make it appear in front of the player

    switch (currentDirection) {
        case Up : posInit.setY(posInit.y() - 10); posInit.setX(posInit.x() + 7); break;
        case Down : posInit.setY(posInit.y() + 5); posInit.setX(posInit.x() + 7); break;
        case Left : posInit.setX(posInit.x() - 2); break;
        case Right : posInit.setX(posInit.x() + 8); break;
        default: break;
    }
    PlayerProjectile* projectile = new PlayerProjectile(0,3, 200, "../assets/images/characters/Missile_spellGrow.gif", posInit, direction);

    projectile->setZValue(10);
    projectile->setScale(0.5);
    scene->addItem(projectile);
    return projectile;
}


///###################PROJECTILEPLAYER METHODS######################

PlayerProjectile::PlayerProjectile(int damage, int speed, int distanceMax, QString spriteSheet, QPointF pos, QPointF direction)
    :Projectile(damage, speed, distanceMax, spriteSheet, pos, direction) {
    frameWidth = 32;
    frameHeight = 21;
    QPointF centerOffset(frameWidth / 2, frameHeight / 2);
    this->setPos(pos - centerOffset);
    throwProjectile();
    qDebug() << "Projectile created at position : " << pos;
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
    //Add a singleshot timer
}

void PlayerProjectile::setEndAnimation(QString spriteSheet, int frameCount, int animationSpeed) {
    if(movie){
        movie->stop();
        delete movie;
        movie = nullptr;
    }
    this->movie = new QMovie(PATH_PLAYER_PROJECTILE_FADE);
    this->movie->start();
    //Add a singleshot timer
    frameCount = 5;
    animationSpeed = 100;
    QTimer::singleShot(frameCount*animationSpeed, this, &PlayerProjectile::deleteProjectile);
}

void PlayerProjectile::startMove() {
    setMiddleAnimation(PATH_PLAYER_PROJECTILE);
    //Starting the moving

    connect(this->timer, &QTimer::timeout, this, &Projectile::move);
    this->timer->start(30);

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
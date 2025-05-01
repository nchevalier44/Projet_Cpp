#include "Player.h"
#include <QTimer>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QAbstractAnimation>
#include <QThread>



Player::Player(std::string name, int life, QGraphicsItem* parent) : Entity(name, life, parent) {
    this->maxHp = life;
    setAnimation(PATH_PLAYER_FRONT_IDLE, 8, 100);
    movingSound = new QSoundEffect(this);
    movingSound->setSource(QUrl::fromLocalFile(PATH_PLAYER_FOOTSTEP_SOUND));
    movingSound->setLoopCount(QSoundEffect::Infinite);
    movingSound->setVolume(30);
}


void Player::takeDamage(int damage) {
    if(isDead) return;

    //Create a red screen to indicate damage
    QWidget* DamageScreen = new QWidget(mainView);
    DamageScreen->setStyleSheet("background-color: red;");
    DamageScreen->setGeometry(mainView->rect());
    DamageScreen->show();

    //Fade of black background
    QGraphicsOpacityEffect* fadeBackgroundEffect = new QGraphicsOpacityEffect(DamageScreen);
    DamageScreen->setGraphicsEffect(fadeBackgroundEffect);
    QPropertyAnimation* animationBackground = new QPropertyAnimation(fadeBackgroundEffect, "opacity", DamageScreen);
    animationBackground->setDuration(150);
    animationBackground->setStartValue(0);
    animationBackground->setEndValue(0.5);
    animationBackground->start(QAbstractAnimation::DeleteWhenStopped);
    QTimer::singleShot(150, [DamageScreen]() {
        DamageScreen->deleteLater();
    });

    setHp(hp - damage);
    if(hud != nullptr) hud->getHPWidget()->setLife(hp);
    if(hp <= 0){
        isDead = true;
        QTimer::singleShot(1000, mainView, &MainView::displayDeathScreen);
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
        case Up : posInit.setY(posInit.y() - 12); posInit.setX(posInit.x() + 7); break;
        case Down : posInit.setY(posInit.y() + 7); posInit.setX(posInit.x() + 7); break;
        case Left : posInit.setX(posInit.x() - 2); break;
        case Right : posInit.setX(posInit.x() + 15); break;
        default: break;
    }
    PlayerProjectile* projectile = new PlayerProjectile(0,3, 200, PATH_MISSILE_SPELL_GROW_ANIMATION, posInit, direction);

    projectile->setZValue(10);
    projectile->setScale(0.5);
    scene->addItem(projectile);


    return projectile;
}

bool Player::canShoot(QPointF clickPos){
    //The player can only shoot in a 90° angle in front of him
    //We check if the player can shoot
    QPointF playerDir;
    QPointF playerPos = this->pos();
    switch(this->getCurrentDirection()) {
        case Up: playerDir = QPointF(0, -1); break;
        case Down: playerDir = QPointF(0, 1); break;
        case Left: playerDir = QPointF(-1, 0); break;
        case Right: playerDir = QPointF(1, 0); break;
        default: return false; // No direction, no shooting
    }

    QPointF dirClick = clickPos - playerPos;
    qreal lenClick = std::hypot(dirClick.x(), dirClick.y());
    if(lenClick == 0) return false; // No click position
    QPointF normClick = dirClick / lenClick;

    qreal dot = normClick.x() * playerDir.x() + normClick.y() * playerDir.y();
    qreal angleDeg = qRadiansToDegrees(qAcos(dot));


    if(angleDeg <= 45.0) {
        //TODO : add a sound to indicate the player shoot
        return true;
    }
    else{
        //TODO : add a sound to indicate the player can't shoot
        return false;
    }
}


///###################PROJECTILEPLAYER METHODS######################

PlayerProjectile::PlayerProjectile(int damage, int speed, int distanceMax, QString spriteSheet, QPointF pos, QPointF direction, QGraphicsObject* parent)
    :Projectile(damage, speed, distanceMax, spriteSheet, pos, direction, parent) {
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
    this->movie = new QMovie(this);
    this->movie->setFileName(spriteSheet);
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
    this->movie = new QMovie(this);
    this->movie->setFileName(spriteSheet);
    this->movie->start();
    //Add a singleshot timer
}

void PlayerProjectile::setEndAnimation(QString spriteSheet, int frameCount, int animationSpeed) {
    if(movie){
        movie->stop();
        delete movie;
        movie = nullptr;
    }
    this->movie = new QMovie(this);
    this->movie->setFileName(PATH_PLAYER_PROJECTILE_FADE);
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
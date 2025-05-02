#include "Player.h"
#include <QTimer>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QAbstractAnimation>
#include <QThread>
#include "../../core/GameScene.h"

Player::Player(std::string name, int life, GameScene* scene, QGraphicsItem* parent) : Entity(name, life, scene, parent) {
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

//void Player::shootProjectile(QPointF target, QGraphicsScene* scene) {
Projectile* Player::shootProjectile(QPointF target, GameScene* scene) {
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
    PlayerProjectile* projectile = new PlayerProjectile(1,3, 200, PATH_MISSILE_SPELL_GROW_ANIMATION, posInit, direction, scene, this);

    projectile->setZValue(10);
    projectile->setScale(0.5);
    scene->addItem(projectile);

    //TODO : undo this line
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

    if(angleDeg < 45){
        return true;
    }
    else{

        return false;
    }
}

void Player::slashAttack(QPointF target, QGraphicsScene* scene){
    QPointF direction = target - this->pos();
    slash->slashAttack(target, this->pos(), currentDirection);
}

void Player::moveEntity(qreal dx, qreal dy){
    qreal posX = this->pos().x();
    qreal posY = this->pos().y();

    //Now we move the player considering eventual collision
    QList<QGraphicsItem*> collisions;
    int numberCollisions;
    int i;

    //Check if there is horizontal collision with an object
    this->setX(posX + dx);
    collisions = this->collidingItems();
    numberCollisions = collisions.count();
    i = 0;
    while(i < numberCollisions && collisions[i]->data(0) != "collision"){
        i++;
    }
    if(i != numberCollisions){
        this->setX(posX);
    }

    //Check if there is vertical collision with an object
    this->setY(posY + dy);
    collisions = this->collidingItems();
    numberCollisions = collisions.count();
    i = 0;
    while(i < numberCollisions && collisions[i]->data(0) != "collision"){
        i++;
    }
    if(i != numberCollisions){
        this->setY(posY);
    }
    mainView->centerOn(this);
}
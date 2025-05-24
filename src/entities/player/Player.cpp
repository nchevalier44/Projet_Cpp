#include "Player.h"
#include <QTimer>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QAbstractAnimation>
#include <QThread>
#include "../../core/GameScene.h"


Player::Player(std::string name, int life, ScoreManager* scoreManager, GameScene* scene, QGraphicsItem* parent) : Entity(name, life, scoreManager, scene, parent) {
    this->maxHp = life;
    setAnimation(PATH_PLAYER_FRONT_IDLE, 8, 100);
    pathDeathSound = PATH_PLAYER_DEATH_SOUND;
    pathHitSound = PATH_PLAYER_HIT_SOUND;
}

void Player::deathAnimation() {
    setAnimation(PATH_PLAYER_DEATH, NB_FRAME_PLAYER_DIE, ANIM_SPEED_PLAYER_IDLE);
    QTimer::singleShot((NB_FRAME_PLAYER_DIE-1)*ANIM_SPEED_PLAYER_IDLE, this, [this]() {
        stopAnimation();
        delete this;
    });
}

void Player::takeDamage(int damage, Entity* attacker) {
    if(isDead) return;

    if(attacker){
        this->takeKnockback(attacker);
    }

    hitSound();

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
        deathAnimation();
        deathSound();
        QTimer::singleShot(1000, mainView, &MainView::displayDeathScreen);
        scoreManager->getActualScore()->setTimePlayed(scoreManager->getElapsedTimer()->elapsed() / 1000);
        scoreManager->getActualScore()->setDate(QDateTime::currentDateTime().toString("dd/MM/yyyy"));
        scoreManager->addScore(*(scoreManager->getActualScore()));
    }
}

void Player::shootProjectile(QPointF target, GameScene* scene) {
    //Ajust the position of the projectile
    QPointF posInit = this->getCenterPosition();

    //Adjusting the position of the projectile to make it appear in front of the player
    qreal w = this->sceneBoundingRect().width();
    qreal h = this->sceneBoundingRect().height();
    switch (currentDirection) {
        case Up : posInit.setX(posInit.x() - w*0.32); posInit.setY(posInit.y() - h*0.8); break;
        case Down : posInit.setX(posInit.x() - w*0.32); posInit.setY(posInit.y()); break;
        case Left : posInit.setX(posInit.x() - w*0.7); posInit.setY(posInit.y() - h * 0.35); break;
        case Right : posInit.setX(posInit.x()); posInit.setY(posInit.y() - h * 0.35); break;
        default: break;
    }

    QPointF direction = target - posInit;


    PlayerProjectile* projectile = new PlayerProjectile(3,5, 400, PATH_MISSILE_SPELL_GROW_ANIMATION, posInit, direction, scene, this);

    projectile->setZValue(41);
    projectile->setScale(0.5);
    scene->addItem(projectile);

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

    if(angleDeg < 75){
        return true;
    }
    else{
        return false;
    }
}

void Player::slashAttack(QPointF target, QGraphicsScene* scene){
    slash->slashAttack(target, this->getCenterPosition(), currentDirection);
}

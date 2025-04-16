#include "Player.h"
#include <QTimer>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QAbstractAnimation>


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

void Player::shootProjectile(QPointF target, QGraphicsScene* scene) {
    //Ajust the position of the projectile
    QPointF posInit = this->pos();
    posInit.setX(posInit.x() + frameWidth/20);
    posInit.setY(posInit.y() + frameHeight/20);
    QPointF direction = target - posInit;
    Projectile* projectile = new Projectile(0,1, 200, "../assets/images/characters/Missile_spell.gif", posInit, direction);

    projectile->setZValue(10);
    projectile->setScale(0.5);
    scene->addItem(projectile);
}
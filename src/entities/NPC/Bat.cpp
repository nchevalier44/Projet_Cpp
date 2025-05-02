#include "Bat.h"
#include "../../core/GameScene.h"

Bat::Bat(std::string name, int life, GameScene* scene, QGraphicsItem* parent) : Entity(name, life, scene, parent) {
    setAnimation(PATH_BAT_MOVE, NB_FRAME_BAT_MOVE, ANIM_SPEED_BAT_MOVE);
    maxHp = life;
    speed = 2;
    rangeAttack = 24;
}

void Bat::deathAnimation() {
    int nbFrame = 12;
    int animSpeed = 100;
    setAnimation(PATH_BAT_DEATH, nbFrame, animSpeed);
    QTimer::singleShot(nbFrame*animSpeed, this, [this]() {
        stopAnimation();
        this->gameScene->removeEntity(this);
    });
}

void Bat::attackAnimation() {
    int nbFrame = 8;
    int animSpeed = 100;
    setAnimation(PATH_BAT_ATTACK, nbFrame, animSpeed);
    if(this->isHorizontalFlipped()) this->horizontalFlip();

    QTimer::singleShot(8*100, this, [=]() {
        this->attacking = false;
        setAnimation(PATH_BAT_MOVE, NB_FRAME_BAT_MOVE, ANIM_SPEED_BAT_MOVE);
        if(this->isHorizontalFlipped()) this->horizontalFlip();
    });
}

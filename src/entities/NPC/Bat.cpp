#include "Bat.h"
#include "../../core/GameScene.h"

Bat::Bat(std::string name, int life, ScoreManager* scoreManager, GameScene* scene, QGraphicsItem* parent) : Entity(name, life, scoreManager, scene, parent) {
    setAnimation(PATH_BAT_MOVE, NB_FRAME_BAT_MOVE, ANIM_SPEED_BAT_MOVE);
    maxHp = life;
    speed = 2;
    rangeAttack = 24;
    score = 10;
    this->setScale(2);
}

void Bat::deathAnimation() {
    setAnimation(PATH_BAT_DEATH, NB_FRAME_BAT_DEATH, ANIM_SPEED_BAT_DEATH);
    QTimer::singleShot(NB_FRAME_BAT_DEATH*ANIM_SPEED_BAT_DEATH, this, [this]() {
        stopAnimation();
        this->gameScene->removeEntity(this);
    });
}

void Bat::attackAnimation() {
    setAnimation(PATH_BAT_ATTACK, NB_FRAME_BAT_ATTACK, ANIM_SPEED_BAT_ATTACK);
    if(this->isHorizontalFlipped()) this->horizontalFlip();

    QTimer::singleShot(NB_FRAME_BAT_ATTACK*ANIM_SPEED_BAT_ATTACK, this, [=]() {
        this->attacking = false;
        setAnimation(PATH_BAT_MOVE, NB_FRAME_BAT_MOVE, ANIM_SPEED_BAT_MOVE);
        if(this->isHorizontalFlipped()) this->horizontalFlip();
    });
}

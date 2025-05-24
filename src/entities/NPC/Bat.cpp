#include "Bat.h"
#include "../../core/GameScene.h"

Bat::Bat(std::string name, int life, ScoreManager* scoreManager, GameScene* scene, QGraphicsItem* parent) : Entity(name, life, scoreManager, scene, parent) {
    setAnimation(PATH_BAT_MOVE, NB_FRAME_BAT_MOVE, ANIM_SPEED_BAT_MOVE);
    maxHp = life;
    speed = 3;
    rangeAttack = 50;
    score = 10;
    this->setScale(2);
    pathDeathSound = PATH_BAT_DEATH_SOUND;
    pathHitSound = PATH_BAT_HIT_SOUND;
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

    QTimer::singleShot(NB_FRAME_BAT_ATTACK*ANIM_SPEED_BAT_ATTACK, this, [=]() {
        this->attacking = false;
        setAnimation(PATH_BAT_MOVE, NB_FRAME_BAT_MOVE, ANIM_SPEED_BAT_MOVE);
    });
}

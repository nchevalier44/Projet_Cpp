#include "Bat.h"

Bat::Bat(std::string name, int life) : Entity(name, life) {
    this->maxHp = life;
    setAnimation(PATH_BAT_MOVE, 9, 90);
    speed = 2;
    rangeAttack = 24;
}

void Bat::deathAnimation() {
    setAnimation(PATH_BAT_DEATH, 12, 100);
    QTimer::singleShot(13*100, this, [=]() {
        stopAnimation();
        delete this;
    });
}

void Bat::attackAnimation() {
    setAnimation(PATH_BAT_ATTACK, 8, 100);
    if(this->isHorizontalFlipped()){
        this->horizontalFlip();
    }
    QTimer::singleShot(8*100, this, [=]() {
        this->attacking = false;
        setAnimation(PATH_BAT_MOVE, 9, 90);
        if(this->isHorizontalFlipped()){
            this->horizontalFlip();
        }
    });
}

#include "Bat.h"

Bat::Bat(std::string name, int life) : Entity(name, life) {
    this->maxHp = life;
    setAnimation("../assets/images/bat/Bat-IdleFly.png", 9, 100);
}

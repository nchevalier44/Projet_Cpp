#include "Player.h"
#include <QTimer>
#include <QVBoxLayout>


Player::Player(std::string name, int life) : Entity(name, life) {
    this->maxHp = life;
    setAnimation("../assets/images/characters/Front_idle.png", 8, 100);

}





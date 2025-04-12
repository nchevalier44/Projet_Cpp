#include "Player.h"
#include <QTimer>


Player::Player(std::string name, int life) : Entity(name, life) {
    this->maxHp = life;
    setAnimation("../assets/images/characters/Front_idle.png", 8, 100);
}

void Player::setHp(int newHp){
    if(newHp <= 0){
        newHp = 0;
        isDead = true;
        QTimer::singleShot(1000, mainView, &MainView::displayDeathScreen);
    }
    hp = newHp;
    hud->getHPWidget()->setLife(hp);
}




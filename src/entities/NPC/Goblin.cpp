#include "Goblin.h"
#include "../Projectile.h"
#include "../../core/GameScene.h"
#include "GoblinProjectile.h"

Goblin::Goblin(std::string name, int life, ScoreManager* scoreManager, GameScene* scene, QGraphicsItem* parent) : Entity(name, life, scoreManager, scene, parent) {
    setAnimation(PATH_GOBLIN_IDLE, NB_FRAME_GOBLIN_IDLE, ANIM_SPEED_GOBLIN_IDLE);
    maxHp = life;
    speed = 2;
    rangeAttack = 200;
    score = 10;
    horizontalFlipped = true;
    this->setScale(0.1);
}

void Goblin::attackAnimation() {
    setAnimation(PATH_GOBLIN_ATTACK, NB_FRAME_GOBLIN_ATTACK, ANIM_SPEED_GOBLIN_ATTACK);

    QTimer::singleShot(NB_FRAME_GOBLIN_ATTACK*ANIM_SPEED_GOBLIN_ATTACK, this, [=]() {
        this->attacking = false;
        setAnimation(PATH_GOBLIN_MOVE, NB_FRAME_GOBLIN_MOVE, ANIM_SPEED_GOBLIN_MOVE);
    });
}

void Goblin::deathAnimation() {
    stopAnimation();
    QTimer::singleShot(200, this, [this]() { //We put a timer else it crash
        stopAnimation();
        this->gameScene->removeEntity(this);
    });
}

void Goblin::moveAnimation() {
    setAnimation(PATH_GOBLIN_MOVE, NB_FRAME_GOBLIN_MOVE, ANIM_SPEED_GOBLIN_MOVE);
}

void Goblin::idleAnimation(){
    setAnimation(PATH_GOBLIN_IDLE, NB_FRAME_GOBLIN_IDLE, ANIM_SPEED_GOBLIN_IDLE);
}

void Goblin::shootProjectile(QPointF target, GameScene* scene) {
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

    GoblinProjectile* projectile = new GoblinProjectile(1,3, 400, nullptr, posInit, direction, scene, this);
    scene->addProjectile(projectile);
    projectile->setZValue(10);
    projectile->setScale(0.5);
    scene->addItem(projectile);

}

void Goblin::attackEntity(Entity* entity) {
    attacking = true;
    this->attackAnimation();
    shootProjectile(entity->getCenterPosition(), this->gameScene);;
}
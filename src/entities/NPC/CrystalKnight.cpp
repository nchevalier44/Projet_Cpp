#include "../../core/GameScene.h"
#include "CrystalKnight.h"



CrystalKnight::CrystalKnight(std::string name, int life, ScoreManager* scoreManager, GameScene* scene, QGraphicsItem* parent) : Entity(name, life, scoreManager, scene, parent) {
    setAnimation(PATH_CK_IDLE, NB_FRAME_CK_IDLE, ANIM_SPEED_CK_IDLE);
    maxHp = life;
    speed = 2;
    score = 100;
    this->setScale(0.3);
}

void CrystalKnight::moveAnimation() {
    setAnimation(PATH_CK_MOVE, NB_FRAME_CK_MOVE, ANIM_SPEED_CK_MOVE);
    QTimer::singleShot(NB_FRAME_CK_MOVE*ANIM_SPEED_CK_MOVE, this, [this]() {
        stopAnimation();
        this->gameScene->removeEntity(this);
    });
}

void CrystalKnight::lightingAnimation(){
    setAnimation(PATH_CK_THUNDER, NB_FRAME_CK_THUNDER, ANIM_SPEED_CK_THUNDER);
    QTimer::singleShot(NB_FRAME_CK_THUNDER*ANIM_SPEED_CK_THUNDER, this, [this]() {
        stopAnimation();
        this->gameScene->removeEntity(this);
    });
}

void CrystalKnight::lightingAttck(){

}


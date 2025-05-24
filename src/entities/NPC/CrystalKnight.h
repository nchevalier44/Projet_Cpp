

#ifndef PROJET_CPP_CRYSTALKNIGHT_H
#define PROJET_CPP_CRYSTALKNIGHT_H

#include "../Entity.h"
#include "../../constants.h"

class CrystalKnight : public Entity {

private :
    int numberLightning = 5;
    QRectF zone;
    bool isDead = false;


public:
    CrystalKnight(std::string name="CrystalKnight", int life=100, ScoreManager* scoreManager=nullptr, GameScene* scene=nullptr, QGraphicsItem* parent=nullptr);
    virtual ~CrystalKnight(){}
    void attackAnimation() override{}
    void deathAnimation() override{}
    void moveAnimation() override;
    void attackEntity(Entity* entity) override{}
    void takeDamage(int damage, Entity* attacker=nullptr, Projectile* projectile=nullptr) override{}

    void lightingAnimation();
    void lightingAttck();







};

#endif //PROJET_CPP_CRYSTALKNIGHT_H

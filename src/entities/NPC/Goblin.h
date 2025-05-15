#ifndef PROJET_CPP_GOBLIN_H
#define PROJET_CPP_GOBLIN_H

#include "../Entity.h"

class Goblin : public Entity {
    public:
    Goblin(std::string name="Goblin", int life=100, ScoreManager* scoreManager=nullptr, GameScene* scene=nullptr, QGraphicsItem* parent=nullptr);

    QPainterPath shape() const{
        QPainterPath path;
        path.addRect(frameWidth*0.125, frameHeight*0.125, frameWidth*0.75, frameHeight*0.75);
        return path;
    }

    void attackAnimation() override;
    void attackEntity(Entity* entity) override;
    void moveAnimation() override;
    void deathAnimation() override;


        void shootProjectile(QPointF target, GameScene* scene);

};


#endif //PROJET_CPP_GOBLIN_H

#ifndef PROJET_CPP_BAT_H
#define PROJET_CPP_BAT_H

#include "../Entity.h"

class Bat : public Entity{
public:
    Bat(std::string name="Bat", int life=100, GameScene* scene=nullptr, QGraphicsItem* parent=nullptr);

    QPainterPath shape() const{
        QPainterPath path;
        path.addRect(frameWidth*0.3, frameHeight*0.3, frameWidth*0.4, frameHeight*0.4);
        return path;
    }

    void deathAnimation() override;
    void attackAnimation() override;

    };


#endif //PROJET_CPP_BAT_H

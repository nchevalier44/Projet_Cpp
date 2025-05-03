#ifndef PROJET_CPP_PLAYERPROJECTILE_H
#define PROJET_CPP_PLAYERPROJECTILE_H

#include "../Projectile.h"
#include "../../constants.h"

class PlayerProjectile : public Projectile {
public :
    PlayerProjectile(int damage, int speed, int distanceMax, QString spriteSheet, QPointF pos, QPointF direction, GameScene* scene, Entity* proprietary, QGraphicsObject* parent=nullptr);
    ~PlayerProjectile() {}
    void throwProjectile();


    void setStartAnimation(QString spriteSheet, int frameCount=0, int animationSpeed=0) override;
    void setMiddleAnimation(QString spriteSheet, int frameCount=0, int animationSpeed=0) override;
    void setEndAnimation(QString spriteSheet, int frameCount, int animationSpeed) override;

public slots :
    void startMove() override;

private:
    QTimer* timerEndMovie = nullptr;
};

#endif //PROJET_CPP_PLAYERPROJECTILE_H

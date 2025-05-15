#ifndef PROJET_CPP_GOBLINPROJECTILE_H
#define PROJET_CPP_GOBLINPROJECTILE_H

#include "../Projectile.h"
#include "../../constants.h"

class GoblinProjectile : public Projectile {
    public :
        GoblinProjectile(int damage, int speed, int distanceMax, QString spriteSheet, QPointF pos, QPointF direction, GameScene* scene, Entity* proprietary=nullptr, QGraphicsObject* parent=nullptr);
        ~GoblinProjectile() {}
        void throwProjectile();

        void setEndAnimation(QString spriteSheet, int frameCount, int animationSpeed) override;

    public slots :
        void startMove() override;

    private:
        QTimer* timerEndMovie = nullptr;
};


#endif //PROJET_CPP_GOBLINPROJECTILE_H
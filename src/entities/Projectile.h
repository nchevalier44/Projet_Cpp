#ifndef PROJET_CPP_PROJECTILE_H
#define PROJET_CPP_PROJECTILE_H

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QMovie>
#include <QGraphicsObject>
#include <QTimer>
#include <QSoundEffect>

class Entity;
class GameScene;

class Projectile : public QGraphicsObject{
Q_OBJECT
protected:
    int damage = 1;
    int speed = 0;
    int distanceMax = 0;
    int distanceTravelled = 0;
    int frameWidth = 0;
    int frameHeight = 0;
    qreal rotationAngle = 0;
    qreal dy=0;
    qreal dx=0;
    qreal erreur = 0.1;
    QMovie* movie = nullptr;
    GameScene* gameScene = nullptr;
    bool isBeenDeleting = false;
    Entity* proprietary = nullptr;
    QString pathMissileMoveSound;
    QString pathMissileBlowSound;
    QSoundEffect* missileMoveSFX = nullptr;


public :
    Projectile(int damage, int speed, int distanceMax, QString spriteSheet, QPointF pos, QPointF direction, GameScene* scene, Entity* proprietary=nullptr, QGraphicsObject* parent=nullptr);
    ~Projectile();
    QPointF getCenterPosition() const;
    virtual QRectF boundingRect() const override;
    virtual QPainterPath shape() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    virtual void startMove();



    //Getters
    int getDamage() const { return damage; }
    int getSpeed() const { return speed; }
    int getDistanceMax() const { return distanceMax; }
    int getDistanceTravelled() const { return distanceTravelled; }

    //Setters
    void setDamage(int newDamage) { damage = newDamage; }
    void setSpeed(int newSpeed) { speed = newSpeed; }
    void setDistanceMax(int newDistanceMax) { distanceMax = newDistanceMax; }
    void setDistanceTravelled(int newDistanceTravelled) { distanceTravelled = newDistanceTravelled; }


    //Animation
    void setStartAnimation(QString spriteSheet);
    void setMiddleAnimation(QString spriteSheet);
    virtual void setEndAnimation(QString spriteSheet, int frameCount, int animationSpeed){}

    void missileMoveSound();
    void missileBlowSound();

public slots:
    void moveProjectile();

};

#endif //PROJET_CPP_PROJECTILE_H

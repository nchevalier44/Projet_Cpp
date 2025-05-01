#ifndef PROJET_CPP_ENTITY_H
#define PROJET_CPP_ENTITY_H

#include "../constants.h"

#include <string>
#include <QPixmap>
#include <QTimer>
#include <QGraphicsPixmapItem>
#include <QPainter>
#include <QString>
#include <QDebug>
#include <QMovie>



class Entity  : public QGraphicsObject {
    Q_OBJECT
public:
    Entity(std::string name="Default Name", int hp=100, QGraphicsItem* parent=nullptr);
    ~Entity();

    //Getters
    int getHp() const{ return hp; }
    int getSpeed() const { return speed; }
    int getMaxHp() const { return maxHp; }
    QPointF getCenterPosition() const;
    Direction getCurrentDirection() const { return currentDirection; }
    QPixmap* getSpriteSheet() { return spriteSheet; }
    int getRangeAttack() const  { return rangeAttack; }
    int getDamage() const { return damage; }
    bool isAttacking() const { return attacking; }
    bool isHorizontalFlipped() const { return horizontalFlipped; }

    //Setters
    virtual void setHp(int newHp) { hp = newHp; }
    void setSpeed(int newSpeed) { speed = newSpeed; }
    void setCurrentDirection(Direction newDirection) { currentDirection = newDirection; }
    void setSpriteSheet(QPixmap* newPixmap) { spriteSheet = newPixmap; }
    void setCenterPosition(QPointF newPos);
    void setHorizontalFlip(bool flip) { horizontalFlipped = flip; }

    //Attack Method
    void attackEntity(Entity* entity);
    virtual void takeDamage(int damage);

    //Other methods
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    //Redefining the boundingRect method
    virtual QRectF boundingRect() const override;

    //Animation method
    void stopAnimation();
    virtual void deathAnimation() {};
    virtual void attackAnimation() {};
    void horizontalFlip();

    //Method to set the animation
    void setAnimation(QString spriteSheet, int frameCount, int animationSpeed);

    //Method to call setAnimation with the right parameters
    //Will make the main code easier to read

    virtual void hpAnimation(){}



private slots :
    void updateAnimation();

protected:
    //Entity variables
    int maxHp;
    int hp;
    std::string name;
    int speed=1;
    Direction currentDirection = None;
    int rangeAttack = 32;
    int damage = 1;
    bool attacking = false;
    bool horizontalFlipped = false;


    //Animation variables
    QPixmap* spriteSheet = nullptr;
    QString currentSpriteSheetPath;
    QTimer* timer = nullptr;
    int currentFrame = 0;
    int frameCount = 0;
    int frameWidth = 0;
    int frameHeight = 0;
    int animationSpeed = 100;
};

class Projectile : public QGraphicsObject{
    Q_OBJECT
protected:
    int damage = 0;
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
    QTimer * timer = nullptr;


public :
    Projectile(int damage, int speed, int distanceMax, QString spriteSheet, QPointF pos, QPointF direction, QGraphicsObject* parent);
    ~Projectile(){}
    void deleteProjectile();
    virtual QRectF boundingRect() const override;
    virtual QPainterPath shape() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;



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
    virtual void setStartAnimation(QString spriteSheet, int frameCount, int animationSpeed){}
    virtual void setMiddleAnimation(QString spriteSheet, int frameCount, int animationSpeed){}
    virtual void setEndAnimation(QString spriteSheet, int frameCount, int animationSpeed){}

    public slots:
    void move();



};


#endif //PROJET_CPP_ENTITY_H

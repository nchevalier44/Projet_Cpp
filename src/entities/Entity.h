#ifndef PROJET_CPP_ENTITY_H
#define PROJET_CPP_ENTITY_H

#include <string>
#include <QPixmap>
#include <QTimer>
#include <QGraphicsPixmapItem>
#include <QPainter>
#include <QString>
#include <QDebug>
#include <QMovie>



class Entity  : public QGraphicsObject{
    Q_OBJECT
public:
    Entity(std::string name="Default Name", int hp=100);

    //Getters
    int getHp() const{ return hp; }
    std::string getName() const { return name; }
    int getSpeed() const { return speed; }
    int getMaxHp() const { return maxHp; }

    //Setters
    virtual void setHp(int newHp) { hp = newHp; }
    void setMaxHp(int newMaxHp) { maxHp = newMaxHp; }
    void setName(std::string newName) { name = newName; }
    void setSpeed(int newSpeed) { speed = newSpeed; }

    //Other methods
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    //Redefining the boundingRect method
    virtual QRectF boundingRect() const override;

    //Animation method
    void stopAnimation();

    //Method to set the animation
    void setAnimation(QString spriteSheet, int frameCount, int animationSpeed);

    //Method to call setAnimation with the right parameters
    //Will make the main code easier to read

    virtual void frontIdleAnimation(){}
    virtual void frontWalkAnimation(){}
    virtual void leftIdleAnimation(){}
    virtual void leftWalkAnimation(){}
    virtual void rightIdleAnimation(){}
    virtual void rightWalkAnimation(){}
    virtual void backIdleAnimation(){}
    virtual void backWalkAnimation(){}
    virtual void hpAnimation(){}



private slots :
    void updateAnimation();

protected:
    //Entity variables
    int maxHp;
    int hp;
    std::string name;
    int speed=1;


    //Animation variables
    QPixmap* spriteSheet;
    QString currentSpriteSheetPath;
    QTimer* timer;
    int currentFrame;
    int frameCount;
    int frameWidth;
    int frameHeight;
    int animationSpeed;
};

class Projectile : public QGraphicsObject{
    Q_OBJECT
private :
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
    Projectile(int damage, int speed, int distanceMax, QString spriteSheet, QPointF pos, QPointF direction);
    ~Projectile(){}
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;



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

    public slots:
    void move();



};


#endif //PROJET_CPP_ENTITY_H

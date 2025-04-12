#ifndef PROJET_CPP_ENTITY_H
#define PROJET_CPP_ENTITY_H

#include <string>
#include <QPixmap>
#include <QTimer>
#include <QGraphicsPixmapItem>
#include <QPainter>
#include <QString>
#include <QDebug>

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
    void setHp(int new_hp) { hp = new_hp; }
    void setMaxHp(int new_max_hp) { maxHp = new_max_hp; }
    void setName(std::string new_name) { name = new_name; }
    void setSpeed(int new_speed) { speed = new_speed; }

    //Other methods
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    //Redefining the boundingRect method
    QRectF boundingRect() const override;

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


#endif //PROJET_CPP_ENTITY_H

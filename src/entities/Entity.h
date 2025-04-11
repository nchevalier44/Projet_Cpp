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

    //Setters
    void setHp(int new_hp) { hp = new_hp; }
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

    void frontIdleAnimation(){
        setAnimation("../assets/images/characters/Front_idle.png", 8, 100);
    }
    void frontWalkAnimation(){
        setAnimation("../assets/images/characters/Front_walk.png", 6, 100);
    }
    void leftIdleAnimation(){
        setAnimation("../assets/images/characters/Left_idle.png", 6, 100);
    }
    void leftWalkAnimation(){
        setAnimation("../assets/images/characters/Left_walk.png", 6, 100);
    }
    void rightIdleAnimation(){
        setAnimation("../assets/images/characters/Right_idle.png", 6, 100);
    }
    void rightWalkAnimation(){
        setAnimation("../assets/images/characters/Right_walk.png", 6, 100);
    }
    void backIdleAnimation(){
        setAnimation("../assets/images/characters/Back_idle.png", 8, 100);
    }
    void backWalkAnimation(){
        setAnimation("../assets/images/characters/Back_walk.png", 8, 100);
    }



private slots :
    void updateAnimation();

private:
    int hp;
    std::string name;
    int speed=1;

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

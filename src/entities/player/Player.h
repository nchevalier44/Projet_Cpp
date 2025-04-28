#ifndef PROJET_CPP_PLAYER_H
#define PROJET_CPP_PLAYER_H

#include <QLabel>
#include <QHBoxLayout>
#include <QMovie>
#include <QSoundEffect>
#include "../Entity.h"
#include "../../core/MainView.h"
#include "../../core/HUD.h"


class PlayerProjectile : public Projectile {
public :
    PlayerProjectile(int damage, int speed, int distanceMax, QString spriteSheet, QPointF pos, QPointF direction);
    ~PlayerProjectile() {}
    void throwProjectile();


    void setStartAnimation(QString spriteSheet, int frameCount=0, int animationSpeed=0) override;
    void setMiddleAnimation(QString spriteSheet, int frameCount=0, int animationSpeed=0) override;
    void setEndAnimation(QString spriteSheet, int frameCount, int animationSpeed) override;

        public slots :
    void startMove();

};


class Player : public Entity {
private :
    MainView* mainView = nullptr;

    bool isDead = false;
    QSoundEffect* movingSound = nullptr;
    Direction currentDirection = Down;

public :

    Player(std::string name = "Player", int life = 100);

    //Getters
    MainView* getMainView() const { return mainView; }
    bool isPlayerDead() const { return isDead; }
    Direction getCurrentDirection() const { return currentDirection; }

    //Setters
    void setMainView(MainView* new_main_view) { mainView = new_main_view; }

    void setCurrentDirection(Direction newDirection) { currentDirection = newDirection; }
    //Override bounding rect to reduce hitbox
    QRectF boundingRect() const{
        return QRectF(0, 0, frameWidth, frameHeight);
    }
    QPainterPath shape() const{
        QPainterPath path;
        path.addRect(frameWidth*0.25, frameHeight*0.25, frameWidth*0.5, frameHeight*0.75);
        return path;
    }
    //Take damage
    void takeDamage(int damage);
    //Animation methods

    void frontIdleAnimation(){
        setAnimation("../assets/images/characters/Front_idle.png", 8, 100);
        if(movingSound->isPlaying()) movingSound->stop();
    }
    void frontWalkAnimation(){
        setAnimation("../assets/images/characters/Front_walk.png", 6, 100);
        if(!movingSound->isPlaying()) movingSound->play();
    }
    void leftIdleAnimation(){
        setAnimation("../assets/images/characters/Left_idle.png", 6, 100);
        if(movingSound->isPlaying()) movingSound->stop();
    }
    void leftWalkAnimation(){
        setAnimation("../assets/images/characters/Left_walk.png", 6, 100);
        if(!movingSound->isPlaying()) movingSound->play();
    }
    void rightIdleAnimation(){
        setAnimation("../assets/images/characters/Right_idle.png", 6, 100);
        if(movingSound->isPlaying()) movingSound->stop();
    }
    void rightWalkAnimation(){
        setAnimation("../assets/images/characters/Right_walk.png", 6, 100);
        if(!movingSound->isPlaying()) movingSound->play();
    }
    void backIdleAnimation(){
        setAnimation("../assets/images/characters/Back_idle.png", 8, 100);
        if(movingSound->isPlaying()) movingSound->stop();
    }
    void backWalkAnimation(){
        setAnimation("../assets/images/characters/Back_walk.png", 8, 100);
        if(!movingSound->isPlaying()) movingSound->play();
    }
    void hpAnimation(){
        setAnimation("../assets/images/characters/Fire_head.png", 8, 150);
    }

    //Attack
    bool canShoot(QPointF clickPos);
    Projectile* shootProjectile(QPointF target, QGraphicsScene* scene);
};



#endif //PROJET_CPP_PLAYER_H

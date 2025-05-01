#ifndef PROJET_CPP_PLAYER_H
#define PROJET_CPP_PLAYER_H

#include <QLabel>
#include <QHBoxLayout>
#include <QMovie>
#include <QSoundEffect>
#include "../Entity.h"
#include "../../core/MainView.h"
#include "../../core/HUD.h"
#include "PlayerProjectile.h"

class Player : public Entity {
private :
    MainView* mainView = nullptr;
    HUD* hud = nullptr;

    bool isDead = false;
    QSoundEffect* movingSound = nullptr;

public :

    Player(std::string name = "Player", int life = 100, QGraphicsItem* parent=nullptr);

    //Getters
    MainView* getMainView() const { return mainView; }
    bool isPlayerDead() const { return isDead; }

    //Setters
    void setMainView(MainView* new_main_view) { mainView = new_main_view; }
    void setHUD(HUD* newHud) { hud = newHud;}

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
    void takeDamage(int damage) override;
    //Animation methods

    void frontIdleAnimation(){
        setAnimation(PATH_PLAYER_FRONT_IDLE, 8, 100);
        if(movingSound->isPlaying()) movingSound->stop();
    }
    void frontWalkAnimation(){
        setAnimation(PATH_PLAYER_FRONT_WALK, 6, 100);
        if(!movingSound->isPlaying()) movingSound->play();
    }
    void leftIdleAnimation(){
        setAnimation(PATH_PLAYER_LEFT_IDLE, 6, 100);
        if(movingSound->isPlaying()) movingSound->stop();
    }
    void leftWalkAnimation(){
        setAnimation(PATH_PLAYER_LEFT_WALK, 6, 100);
        if(!movingSound->isPlaying()) movingSound->play();
    }
    void rightIdleAnimation(){
        setAnimation(PATH_PLAYER_RIGHT_IDLE, 6, 100);
        if(movingSound->isPlaying()) movingSound->stop();
    }
    void rightWalkAnimation(){
        setAnimation(PATH_PLAYER_RIGHT_WALK, 6, 100);
        if(!movingSound->isPlaying()) movingSound->play();
    }
    void backIdleAnimation(){
        setAnimation(PATH_PLAYER_BACK_IDLE, 8, 100);
        if(movingSound->isPlaying()) movingSound->stop();
    }
    void backWalkAnimation(){
        setAnimation(PATH_PLAYER_BACK_WALK, 8, 100);
        if(!movingSound->isPlaying()) movingSound->play();
    }
    void hpAnimation(){
        setAnimation(PATH_HP_DEAD, 8, 150);
    }

    void moveEntity(qreal dx, qreal dy) override;

    //Attack
    bool canShoot(QPointF clickPos);
    Projectile* shootProjectile(QPointF target, GameScene* scene);
};



#endif //PROJET_CPP_PLAYER_H

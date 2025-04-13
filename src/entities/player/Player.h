#ifndef PROJET_CPP_PLAYER_H
#define PROJET_CPP_PLAYER_H

#include <QLabel>
#include <QHBoxLayout>
#include <QMovie>
#include <QSoundEffect>
#include "../Entity.h"
#include "../../core/MainView.h"
#include "../../core/HUD.h"

class Player : public Entity {
private :
    MainView* mainView = nullptr;
    HUD* hud = nullptr;
    bool isDead = false;
    QSoundEffect* movingSound = nullptr;

public :
    Player(std::string name = "Player", int life = 100);

    //Getters
    MainView* getMainView() const { return mainView; }
    bool isPlayerDead() const { return isDead; }

    //Setters
    void setMainView(MainView* new_main_view) { mainView = new_main_view; }
    virtual void setHp(int newHp){hp = newHp; hud->getHPWidget()->setLife(hp);}
    void setHUD(HUD* newHud) { hud = newHud; }

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
};



#endif //PROJET_CPP_PLAYER_H

#ifndef PROJET_CPP_PLAYER_H
#define PROJET_CPP_PLAYER_H

#include <QLabel>
#include <QHBoxLayout>
#include <QMovie>
#include <QSoundEffect>
#include <QElapsedTimer>
#include <QGraphicsProxyWidget>
#include "../Entity.h"
#include "../../core/MainView.h"
#include "../../core/HUD.h"


class PlayerSlash : public QGraphicsObject {
    Q_OBJECT
private :
    int currentAttackIndex = 0;
    QElapsedTimer combotimer;
    const int comboMaxDelay = 1000;
    QVector<QMovie*> attackAnimation;
    qreal rotationAngle = 0;
    QPointF attackPosition;
    QGraphicsScene* scene = nullptr;
    QPixmap currentPixmap;


public :
    PlayerSlash(QGraphicsScene* scene);
    ~PlayerSlash() {delete attackAnimation[0]; delete attackAnimation[1]; delete attackAnimation[2];}

    void playAttackAnimation(QPointF playerPos);
    void slashAttack(QPointF pos, QPointF playerPos, Direction CurrentDirection);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
};



class PlayerProjectile : public Projectile {
    Q_OBJECT
public :
    PlayerProjectile(int damage, int speed, int distanceMax, QString spriteSheet, QPointF pos, QPointF direction, QGraphicsObject* parent=nullptr);
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
    HUD* hud = nullptr;

    bool isDead = false;
    QSoundEffect* movingSound = nullptr;

    PlayerSlash* slash = nullptr;

public :

    Player(std::string name = "Player", int life = 100, QGraphicsItem* parent=nullptr);

    //Getters
    MainView* getMainView() const { return mainView; }
    bool isPlayerDead() const { return isDead; }
    Direction getCurrentDirection() const { return currentDirection; }
    PlayerSlash* getPlayerSlash() const { return slash; }

    //Setters
    void setMainView(MainView* new_main_view) { mainView = new_main_view; }
    void setPlayerSlash(PlayerSlash* new_slash) { slash = new_slash; }
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

    //Attack
    bool canShoot(QPointF clickPos);
    void shootProjectile(QPointF target, QGraphicsScene* scene);
    void slashAttack(QPointF target, QGraphicsScene* scene);
};



#endif //PROJET_CPP_PLAYER_H

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
#include "PlayerShield.h"

#include "PlayerProjectile.h"
#include "PlayerSlash.h"
#include "PlayerShield.h"

class Player : public Entity
{
private:
    MainView *mainView = nullptr;
    HUD *hud = nullptr;

    bool hasSlash = false;
    bool hasMissile = false;
    bool hasShield = false;
    bool hasTreeHeart = false; // For the Crystal Knight fight

    PlayerSlash* slash = nullptr;
    PlayerShield* shield = nullptr;

public:
    Player(std::string name = "Player", int life = 100, ScoreManager* scoreManager=nullptr, GameScene* scene = nullptr, QGraphicsItem* parent = nullptr);
    ~Player(){};

    // Getters
    MainView* getMainView() const { return mainView; }
    Direction getCurrentDirection() const { return currentDirection; }
    PlayerSlash *getPlayerSlash() const { return slash; }
    PlayerShield *getPlayerShield() const { return shield; }
    int getMaxHP() const { return maxHp; }
    int getCurrentHP() const { return hp; }
    bool getHasSlash(){return hasSlash;}
    bool getHasMissile(){return hasMissile;}
    bool getHasShield(){return hasShield;}
    bool getHasTreeHeart(){return hasTreeHeart;}

    // Setters
    void setMainView(MainView *new_main_view) { mainView = new_main_view; }
    void setMaxHP(int newMaxHP) { maxHp = newMaxHP; }
    void setPlayerSlash(PlayerSlash *new_slash) { slash = new_slash; }
    void setPlayerShield(PlayerShield* new_shield) {shield = new_shield; }
    void setHUD(HUD *newHud) { hud = newHud; }
    void setHasShield(bool b) { hasShield = b; }
    void setHasSlash(bool b) { hasSlash = b; }
    void setHasMissile(bool b) { hasMissile = b; }
    void setHasTreeHeart(bool b){hasTreeHeart = b;}

    // Override bounding rect to reduce hitbox
    QRectF boundingRect() const override { return QRectF(0, 0, frameWidth, frameHeight); }
    QPainterPath shape() const override
    {
        QPainterPath path;
        path.addRect(frameWidth * 0.25, frameHeight * 0.25, frameWidth * 0.5, frameHeight * 0.75);
        return path;
    }
    // Take damage
    void takeDamage(int damage, Entity* attacker=nullptr, Projectile* projectile=nullptr) override;
        // Animation methods

    void frontIdleAnimation() { setAnimation(PATH_PLAYER_FRONT_IDLE, 8, 100); }
    void frontWalkAnimation() { setAnimation(PATH_PLAYER_FRONT_WALK, 6, 100); }
    void leftIdleAnimation() { setAnimation(PATH_PLAYER_LEFT_IDLE, 6, 100); }
    void leftWalkAnimation() { setAnimation(PATH_PLAYER_LEFT_WALK, 6, 100); }
    void rightIdleAnimation() { setAnimation(PATH_PLAYER_RIGHT_IDLE, 6, 100); }
    void rightWalkAnimation() { setAnimation(PATH_PLAYER_RIGHT_WALK, 6, 100); }
    void backIdleAnimation() { setAnimation(PATH_PLAYER_BACK_IDLE, 8, 100); }
    void backWalkAnimation() { setAnimation(PATH_PLAYER_BACK_WALK, 8, 100); }

    void hpAnimation() override { setAnimation(PATH_HP_DEAD, 8, 150); }

    void deathAnimation() override;

    // Attack
    bool canShoot(QPointF clickPos);
    void slashAttack(QPointF target, QGraphicsScene *scene);
    void shootProjectile(QPointF target, GameScene *scene);
    void useShield(){shield->activeShield();}
};

#endif // PROJET_CPP_PLAYER_H

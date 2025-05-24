

#ifndef PROJET_CPP_CRYSTALKNIGHT_H
#define PROJET_CPP_CRYSTALKNIGHT_H
#include <QRectF>

#include <QRandomGenerator>
#include "../Entity.h"
#include "../../constants.h"





class CrystalKnight : public Entity {

private :
    int numberLightning = 5;
    QRectF zoneTP;
    QRectF zoneAttack;
    bool isDead = false;
    bool isAttacking = false;
    bool isClawAttacking = false;
    bool clawAttackTouched = false;
    int lightningCount = 0;
    int maxLightningCount = 15;
    QTimer* lightningTimer = nullptr;
    Player* player = nullptr;
    QTimer* clawTimer = nullptr;



public:
    CrystalKnight(std::string name="CrystalKnight", int life=100,Player* player = nullptr, ScoreManager* scoreManager=nullptr, GameScene* scene=nullptr, QGraphicsItem* parent=nullptr);
    virtual ~CrystalKnight(){
        if(lightningTimer) {
            lightningTimer->stop();
            delete lightningTimer;
            lightningTimer = nullptr;
        }
        player = nullptr;
        if(clawTimer) {
            clawTimer->stop();
            delete clawTimer;
            clawTimer = nullptr;
        }
    }
    void attackAnimation() override{}
    void deathAnimation() override;
    void takeDamage(int damage, Entity* attacker=nullptr, Projectile* projectile=nullptr) override{
        Entity::takeDamage(damage, attacker, projectile);
        if(hp == maxHp/2){
            maxLightningCount = 25;
        }
    }
    void moveAnimation(qreal x = 0, qreal y = 0);
    void clawAttackAnimation();
    void checkCollisionsClawAttack();

    void attackLoop();
    void lightningAttck();
    void teleport(qreal x = 0, qreal y = 0);
    void performRandomAction();
    void teleportOnPlayer();


    QPainterPath shape() const override{
        QPainterPath path;
        if(isClawAttacking){
            path.addRect(frameWidth*0.25, frameHeight*0.1, frameWidth, frameHeight);
            return path;
        }
        path.addRect(frameWidth*0.25, frameHeight*0.1, frameWidth*0.5, frameHeight*0.9);
        return path;
    }
};

class Lightning : public QObject, public QGraphicsPixmapItem {
Q_OBJECT
public:
    Lightning(const QRectF& spawnZone,Player* player = nullptr, GameScene* scene = nullptr,QGraphicsItem* parent = nullptr);
    ~Lightning();
    void checkCollisions();
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;


private slots:
    void updateFrame();


private:
    static const int frameCount = 10;
    static const int frameWidth = 200;
    static const int frameHeight = 680;
    static const int animSpeedMs = ANIM_SPEED_CK_THUNDER;  // Durée entre chaque frame (en ms)

    QTimer timer;
    QPixmap spriteSheet;
    int currentFrame = 0;
    QRectF zone;
    GameScene* gameScene = nullptr;
    bool hasCollided = false;
    Player* player = nullptr;
    void setRandomPosition();
};

#endif //PROJET_CPP_CRYSTALKNIGHT_H

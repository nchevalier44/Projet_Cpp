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

class GameScene;

class Entity  : public QGraphicsObject {
    Q_OBJECT
public:
    Entity(std::string name="Default Name", int hp=100, GameScene* scene=nullptr, QGraphicsItem* parent=nullptr);
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
    void setCenterPosition(QPointF newPos){ setPos(newPos - boundingRect().center()); };
    void setCenterX(qreal newX) { setX(newX - boundingRect().center().x()); }
    void setCenterY(qreal newY) { setY(newY - boundingRect().center().y()); }
    void setHorizontalFlip(bool flip) { horizontalFlipped = flip; }

    //Attack Method
    void attackEntity(Entity* entity);
    virtual void takeDamage(int damage);

    //Other methods
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    //Redefining the boundingRect method
    virtual QRectF boundingRect() const override;
    virtual void moveEntity(qreal x, qreal y);

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
    GameScene* gameScene = nullptr;

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


#endif //PROJET_CPP_ENTITY_H

#include <QGraphicsObject>
#include <QMovie>
#include <QElapsedTimer>
#include <QPainter>
#include "../../constants.h"


#ifndef PROJET_CPP_PLAYERSHIELD_H
#define PROJET_CPP_PLAYERSHIELD_H

class Player;
class GameScene;


class PlayerShield : public QGraphicsObject {
Q_OBJECT

private :
    bool isActivated = false;
    QGraphicsScene* scene = nullptr;
    QMovie* movie;
    QElapsedTimer time;
    QPixmap currentPixmap;
    int HP = 3;
    int maxHP = 3;
    const int durability = 2000;
    Player* player;

public :
    PlayerShield(GameScene* scene, Player* player);
    ~PlayerShield(){}

    void activeShield();
    void desactiveShield();
    void updatePosition();
    void decreaseHP(){HP--;if(HP == 0){desactiveShield();}}


    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

};

#endif //PROJET_CPP_PLAYERSHIELD_H

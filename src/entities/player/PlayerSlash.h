#ifndef PROJET_CPP_PLAYERSLASH_H
#define PROJET_CPP_PLAYERSLASH_H

#include <QGraphicsObject>
#include <QElapsedTimer>
#include <QMovie>
#include "../../constants.h"



class GameScene;
class Player;


class PlayerSlash : public QGraphicsObject {
    Q_OBJECT
private :
    int damage = 5;
    int currentAttackIndex = 0;
    QElapsedTimer combotimer;
    const int comboMaxDelay = 1000;
    QVector<QMovie*> attackAnimation;
    qreal rotationAngle = 0;
    QPointF attackPosition;
    GameScene* scene = nullptr;
    QPixmap currentPixmap;
    bool isSlashing = false;
    Player *player = nullptr;
    QTimer* animationTimer = nullptr;


public :
    PlayerSlash(GameScene* scene, Player *player, QGraphicsObject* parent=nullptr);
    ~PlayerSlash() {delete attackAnimation[0]; delete attackAnimation[1]; delete attackAnimation[2];}

    void slashAttack(QPointF pos, QPointF playerPos, Direction CurrentDirection);
    void checkCollide();

    bool getIsSlashing() const { return isSlashing; }
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
};


#endif //PROJET_CPP_PLAYERSLASH_H

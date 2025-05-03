#ifndef PROJET_CPP_PLAYERSLASH_H
#define PROJET_CPP_PLAYERSLASH_H

#include <QGraphicsObject>
#include <QElapsedTimer>
#include <QMovie>
#include "../../constants.h"

class GameScene;


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
    PlayerSlash(GameScene* scene);
    ~PlayerSlash() {delete attackAnimation[0]; delete attackAnimation[1]; delete attackAnimation[2];}

    void playAttackAnimation(QPointF playerPos);
    void slashAttack(QPointF pos, QPointF playerPos, Direction CurrentDirection);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
};


#endif //PROJET_CPP_PLAYERSLASH_H

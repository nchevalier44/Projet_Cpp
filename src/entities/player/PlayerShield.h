#include <QGraphicsObject>
#include <QMovie>
#include <QElapsedTimer>
#include <QPainter>
#include "../../constants.h"


#ifndef PROJET_CPP_PLAYERSHIELD_H
#define PROJET_CPP_PLAYERSHIELD_H

class Shield : public QGraphicsObject {
Q_OBJECT

private :
    bool isActivated;
    QMovie* movie;
    QElapsedTimer timer;
    QPixmap currentPixmap;




public :
    Shield();
    ~Shield(){}

    void activeShield(){}

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

};

#endif //PROJET_CPP_PLAYERSHIELD_H

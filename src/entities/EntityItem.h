#ifndef PROJET_CPP_ENTITYITEM_H
#define PROJET_CPP_ENTITYITEM_H

#include <QGraphicsPixmapItem>
#include <iostream>
#include "Entity.h"

class EntityItem : public QGraphicsPixmapItem {
    public:
        EntityItem(QString imageFileName, Entity* entity);
        void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;


private:
    Entity* entity;
};


#endif //PROJET_CPP_ENTITYITEM_H

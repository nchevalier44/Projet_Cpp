#ifndef PROJET_CPP_ENTITYITEM_H
#define PROJET_CPP_ENTITYITEM_H

#include <QGraphicsPixmapItem>
#include <iostream>
#include "Entity.h"

class EntityItem : public QGraphicsPixmapItem {
    public:
        EntityItem(QString imageFileName, Entity* entity) : QGraphicsPixmapItem(QPixmap(imageFileName)), entity(entity){
            //std::cout << "Character creation" << std::endl;
        }

private:
    Entity* entity;
};


#endif //PROJET_CPP_ENTITYITEM_H

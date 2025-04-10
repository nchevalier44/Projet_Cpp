#include "EntityItem.h"

EntityItem::EntityItem(QString imageFileName, Entity* entity) : entity(entity) {
    std::cout << "Character creation" << std::endl;
}

void EntityItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    if (entity) {
        qDebug() << "EntityItem is delegating paint to Entity";
        entity->paint(painter, option, widget);
    }
}
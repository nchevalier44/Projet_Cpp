#ifndef PROJET_CPP_CHARACTERITEM_H
#define PROJET_CPP_CHARACTERITEM_H

#include <QGraphicsPixmapItem>
#include <iostream>

class CharacterItem : public QGraphicsPixmapItem {
    public:
        CharacterItem(QString imageFileName) : QGraphicsPixmapItem(QPixmap(imageFileName)){
            std::cout << "Character creation" << std::endl;
        }

};


#endif //PROJET_CPP_CHARACTERITEM_H

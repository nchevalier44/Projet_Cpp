#ifndef PROJET_CPP_BAT_H
#define PROJET_CPP_BAT_H

#include "../Entity.h"

class Bat : public Entity{
public:
    Bat(std::string name="Bat", int life=100);

    QRectF boundingRect() const{
        return QRectF(frameWidth*0.2, frameHeight*0.2, frameWidth*0.6, frameHeight*0.6);
    }

    QPainterPath shape() const{
        QPainterPath path;
        path.addRect(frameWidth*0.3, frameHeight*0.3, frameWidth*0.4, frameHeight*0.4);
        return path;
    }



};


#endif //PROJET_CPP_BAT_H

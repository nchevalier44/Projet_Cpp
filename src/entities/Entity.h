#ifndef PROJET_CPP_ENTITY_H
#define PROJET_CPP_ENTITY_H

#include <string>
#include <QPixmap>
#include <QTimer>
#include <QGraphicsPixmapItem>
#include <QPainter>
#include <QString>

class Entity  : public QGraphicsObject{
    Q_OBJECT
public:
    Entity( QString spriteSheet, int frameCount, int animationSpeed, std::string name="Default Name", int hp=100);

    //Getters
    int getHp() const{ return hp; }
    std::string getName() const { return name; }

    //Setters
    void setHp(int new_hp) { hp = new_hp; }
    void setName(std::string new_name) { name = new_name; }

    //Other methods
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    //Redefining the boundingRect method
    QRectF boundingRect() const override;


private slots :
    void updateAnimation();

private:
    int hp;
    std::string name;

    QPixmap* spriteSheet;
    QTimer* timer;
    int currentFrame;
    int frameCount;
    int frameWidth;
    int frameHeight;
    int animationSpeed;


};


#endif //PROJET_CPP_ENTITY_H

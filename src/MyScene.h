

#ifndef PROJET_CPP_MYSCENE_H
#define PROJET_CPP_MYSCENE_H

#include <QGraphicsScene>

class MyScene : public QGraphicsScene {
    Q_OBJECT
public :
    MyScene(QObject* parent = nullptr);
    virtual ~MyScene();


};





#endif //PROJET_CPP_MYSCENE_H

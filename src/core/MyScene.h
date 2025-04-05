#ifndef PROJET_CPP_MYSCENE_H
#define PROJET_CPP_MYSCENE_H

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QTimer>
#include "../CharacterItem.h"
#include <QKeyEvent>

class MyScene : public QGraphicsScene {
    Q_OBJECT

private:
    QTimer* timer;
    CharacterItem* character;

protected:
    void keyPressEvent(QKeyEvent* event);

public :
    MyScene(QObject* parent = nullptr);
    virtual ~MyScene();

public slots:
    void timerUpdate();



};





#endif //PROJET_CPP_MYSCENE_H
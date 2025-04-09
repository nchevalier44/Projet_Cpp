#ifndef PROJET_CPP_GAMESCENE_H
#define PROJET_CPP_GAMESCENE_H

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QTimer>
#include "../entities/EntityItem.h"
#include <QKeyEvent>
#include <QPointF>
#include <QPushButton>

class GameScene : public QGraphicsScene {
    Q_OBJECT

private:
    QTimer* timer;
    Entity* character;

protected:
    void keyPressEvent(QKeyEvent* event);

public :
    GameScene(QObject* parent = nullptr);
    virtual ~GameScene();

public slots:
    void timerUpdate();



};





#endif //PROJET_CPP_GAMESCENE_H
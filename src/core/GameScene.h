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
    QSet<int> activeKeys; // Set to keep track of active keys

protected:
    //Handle the mouvement and animation of the player
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

public :
    GameScene(QObject* parent = nullptr);
    virtual ~GameScene();

public slots:
    void timerUpdate();



};





#endif //PROJET_CPP_GAMESCENE_H
#ifndef PROJET_CPP_GAMESCENE_H
#define PROJET_CPP_GAMESCENE_H

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QTimer>
#include "../entities/Entity.h"
#include <QKeyEvent>
#include <QPointF>
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include "MainView.h"
#include "../entities/player/Player.h"


class GameScene : public QGraphicsScene {
    Q_OBJECT

private:
    enum Direction {None, Up, Down, Left, Right};
    Direction currentDirection = None;

    QTimer* timer = nullptr;
    Player* character = nullptr;
    MainView* mainView = nullptr;
    int backgroundWidth = 0;
    int backgroundHeight = 0;
    QList<int> activeKeys; // Set to keep track of active keys

protected:
    //Handle the mouvement and animation of the player
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

public :

    //Setters
    void setCharacter(Player* new_character) { character = new_character; }
    void setTimer(QTimer* new_timer) { timer = new_timer; }
    void setActiveKeys(const QList<int>& keys) { activeKeys = keys; }
    void setView(MainView* view) { mainView = view; }

    //Getters
    Player* getCharacter() const { return character; }
    QTimer* getTimer() const { return timer; }
    QList<int> getActiveKeys() const { return activeKeys; }

    //Constructor and destructor
    GameScene(MainView* view, QObject* parent = nullptr);
    virtual ~GameScene();

    //Functions
    void loadMap();
    qreal* getDeltaPosition();

    //Mouse interactions
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

        public slots:
    void timerUpdate();

};

#endif //PROJET_CPP_GAMESCENE_H
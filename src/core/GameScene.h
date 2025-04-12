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
    QTimer* timer;
    Player* character;
    MainView* mainView;
    QPixmap background;
    QList<int> activeKeys; // Set to keep track of active keys

protected:
    //Handle the mouvement and animation of the player
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

public :

    //Setters
    void setCharacter(Player* new_character) { character = new_character; }
    void setBackground(const QPixmap& new_background) { background = new_background; }
    void setTimer(QTimer* new_timer) { timer = new_timer; }
    void setActiveKeys(const QList<int>& keys) { activeKeys = keys; }

    //Getters
    Player* getCharacter() const { return character; }
    QPixmap getBackground() const { return background; }
    QTimer* getTimer() const { return timer; }
    QList<int> getActiveKeys() const { return activeKeys; }

    //Constructor and destructor
    GameScene(QObject* parent = nullptr);
    void setView(MainView* view) { mainView = view; }
    void drawBackground(QPainter *painter, const QRectF &rect) override;
    virtual ~GameScene();

public slots:
    void timerUpdate();



};





#endif //PROJET_CPP_GAMESCENE_H
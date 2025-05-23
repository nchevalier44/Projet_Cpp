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
#include <QMediaPlayer>
#include "MainView.h"
#include "../entities/NPC/Bat.h"
#include "../entities/player/Player.h"
#include "../entities/NPC/Goblin.h"
#include "../entities/NPC/CrystalKnight.h"


class MainWindow;

class GameScene : public QGraphicsScene {
    Q_OBJECT

private:
    bool isPaused = false;

    QMediaPlayer* audioPlayer = nullptr;
    QGraphicsPixmapItem* mapItem = nullptr;
    QTimer* timer = nullptr;
    Player* character = nullptr;
    QList<Entity*> listNPC;
    QList<Projectile*> listProjectiles;
    QList<QGraphicsPixmapItem*> listBackground;
    MainView* mainView = nullptr;
    MainWindow* mainWindow = nullptr;
    int backgroundWidth = 0;
    int backgroundHeight = 0;
    QList<int> activeKeys; // Set to keep track of active keys
    HUD* hud = nullptr;
    ScoreManager* scoreManager = nullptr;
    QList<QTimer*> timerList;
    QList<QMovie*> movieList;
    QGraphicsTextItem* tooltiptxt = nullptr;
    QGraphicsRectItem* tooltiprect = nullptr;


protected:
    //Handle the mouvement and animation of the player
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

public :

    void putGamePaused();
    void reverseGamePaused();

    //Setters
    void setCharacter(Player* new_character) { character = new_character; }
    void setTimer(QTimer* new_timer) { timer = new_timer; }
    void setActiveKeys(const QList<int>& keys) { activeKeys = keys; }
    void setView(MainView* view) { mainView = view; }
    virtual void setHp(int newHp){character->setHp(newHp); hud->getHPWidget()->setLife(character->getHp());}
    void setHUD(HUD* newHud) { hud = newHud; }

    void addProjectile(Projectile* projectile) { listProjectiles.append(projectile); }
    void addEntity(Entity* entity) { listNPC.append(entity); }
    void removeProjectile(Projectile* projectile);
    void removeEntity(Entity* entity);

    //Getters
    Player* getCharacter() const { return character; }
    QTimer* getTimer() const { return timer; }
    QList<int> getActiveKeys() const { return activeKeys; }
    QList<QTimer*>& getTimerList() { return timerList; }
    QList<QMovie*>& getMovieList() { return movieList; }
    bool isGamePaused() const { return isPaused; }

    //Constructor and destructor
    GameScene(MainWindow* mainWindow, MainView* view, ScoreManager* scoreManager, QObject* parent = nullptr);
    virtual ~GameScene();

    //Functions
    void loadMap(QString mapPath, int mapWidth, int mapHeight);
    void loadOverworld();
    void loadDungeon();
    void addInteractionZone(QString name, QJsonObject layer);
    void clearInteractionZones(const QStringList& tags);
    qreal* getDeltaPosition();
    void moveNPC();
    void movePlayer();
    void checkNPCAttackRange();
    void moveProjectiles();
    void checkInteractionZone();
    void showTooltip(QPointF pos, QString text);
    void removeTooltip();

    //Mouse interactions
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

        public slots:
    void timerUpdate();

};

#endif //PROJET_CPP_GAMESCENE_H
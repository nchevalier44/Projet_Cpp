#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QException>
#include <QGraphicsSceneMouseEvent>
#include "GameScene.h"
#include "MainWindow.h"
#include "../entities/player/Player.h"
#include "../entities/NPC/Goblin.h"


GameScene::GameScene(MainView* view, ScoreManager* scoreManager, QObject* parent) : QGraphicsScene(parent), scoreManager(scoreManager), mainView(view){

    try{
        loadMap();
    } catch(QException e){
        qCritical() << "Error when loading the map : " << e.what();
    } catch(std::exception e){
        qCritical() << "Error when loading the map : " << e.what();
    }


    this->setSceneRect(0, 0, backgroundWidth, backgroundHeight);

    //Setting up the player's character
    this->character = new Player("Character", 3, scoreManager);
    this->character->setPos(1480, 2730);
    this->character->setSpeed(6);
    this->character->setScale(0.2);
    this->character->setFocus();
    this->mainView->setFocus(); //Set the focus on the mainView so we can detect the key press

    this->addItem(character);
    this->character->setMainView(mainView);

    //Load slash animation
    PlayerSlash* slash = new PlayerSlash(this, character);
    this->character->setPlayerSlash(slash);

    //Load the shield animation
    PlayerShield* shield = new PlayerShield(this, character);
    this->character->setPLayerShield(shield);

    Goblin* goblin = new Goblin("Goblin", 1, scoreManager, this);
    goblin->setPos(1200, 2350);
    this->addItem(goblin);
    listNPC.append(goblin);

    Bat* bat = new Bat("Bat", 1, scoreManager, this);
    bat->setPos(1100, 2200);
    this->addItem(bat);
    listNPC.append(bat);


    //Starting the timer to update the animation and mouvement
    this->timer = new QTimer(this);
    connect(this->timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
    this->timer->start(30); //every 30 milliseconds

}

void GameScene::loadMap(){

    //Load and parse json file
    QFile file("../assets/maps/map.json");
    file.open(QIODevice::ReadOnly);

    QJsonDocument document = QJsonDocument::fromJson(file.readAll());
    QJsonObject mapObject = document.object();

    this->backgroundWidth = 3000;
    this->backgroundHeight = 3000;

    QPixmap mapPixmap(backgroundWidth, backgroundHeight);
    QPainter painter(&mapPixmap);

    QJsonArray layers = mapObject["layers"].toArray();
    for(QJsonValue layerValue : layers){
        QJsonObject layer = layerValue.toObject();

        if(layer["type"] == "imagelayer"){
            QPixmap image("../assets/maps/" + layer["image"].toString());
            if(image.isNull()){
                qDebug() << "Image not found : " << layer["image"].toString();
            }

            painter.setOpacity(layer["opacity"].toDouble());
            painter.drawPixmap(layer["x"].toInt(), layer["y"].toInt(), image);

            //Add collisions objects
        } else if(layer["name"] == "collisions"){
            QJsonArray objects = layer["objects"].toArray();

            for(QJsonValue objectValue : objects){
                QJsonObject object = objectValue.toObject();

                int x = object["x"].toDouble();
                int y = object["y"].toDouble();
                int width = object["width"].toDouble();
                int height = object["height"].toDouble();


                if (object.contains("polygon") || object.contains("polyline")) {
                    QPolygonF polygon;

                    QJsonArray points;
                    if(object.contains("polygon")){
                        points = object["polygon"].toArray();
                    } else{
                        points = object["polyline"].toArray();
                    }

                    for (const QJsonValue& pointValue : points) {
                        QJsonObject point = pointValue.toObject();
                        qreal px = point["x"].toDouble();
                        qreal py = point["y"].toDouble();
                        polygon << QPointF(x + px, y + py); // coordonnées relatives à (x,y)
                    }

                    QGraphicsPolygonItem* polyItem = new QGraphicsPolygonItem(polygon, mapItem);
                    //polyItem->setBrush(Qt::red);
                    polyItem->setPen(Qt::NoPen);
                    polyItem->setData(0, "collision");
                    polyItem->setZValue(100);
                    this->addItem(polyItem);
                } else {
                    QGraphicsRectItem* rect = new QGraphicsRectItem(x, y, width, height, mapItem);
                    //rect->setBrush(Qt::green);
                    rect->setPen(Qt::NoPen);
                    rect->setData(0, "collision");
                    rect->setZValue(100);
                    this->addItem(rect);
                }
            }

        }
    }

    painter.end();
    mapItem = new QGraphicsPixmapItem(mapPixmap);
    this->addItem(mapItem); //Add the map

    file.close();
}

//Mouvement functions
//Adapt the animation according to the direction
void GameScene::keyPressEvent(QKeyEvent* event){
    if(character->isEntityDead()) return;
    if(event->isAutoRepeat()){
        return; //They key stay pressed so the walk animation can continue
    }
    activeKeys.append(event->key());

    switch (event->key()){
        case Qt::Key_A :
            qDebug("Key A pressed");
            hud->getSpellWidget()->changeSelectedSpell(0);
            break;
        case Qt::Key_W :
            qDebug("Key W pressed");
            hud->getSpellWidget()->changeSelectedSpell(1);
            break;
        case Qt::Key_X :
            qDebug("Key X pressed");
            hud->getSpellWidget()->changeSelectedSpell(2);
        case Qt::Key_T:
            qDebug() << character->pos();
            break;
        default:
            break;
    }
}


//Set the idle animation according to the last key pressed
void GameScene::keyReleaseEvent(QKeyEvent *event) {
    if(event->isAutoRepeat()){
        return; //They key stay pressed so the walk animation can continue
    }

    activeKeys.removeAll(event->key());

    if(!activeKeys.isEmpty()){
        return; //We change the animation to idle only if there are no others key pressed
    }
    switch(event->key()){
        case Qt::Key_Up:
        case Qt::Key_Z:
            character->backIdleAnimation();
            break;
        case Qt::Key_Down:
        case Qt::Key_S:
            character->frontIdleAnimation();
            break;
        case Qt::Key_Left:
        case Qt::Key_Q:
            character->leftIdleAnimation();
            break;
        case Qt::Key_Right:
        case Qt::Key_D:
            character->rightIdleAnimation();
            break;
    }
}



void GameScene::timerUpdate(){
    moveNPC();
    movePlayer();
    checkNPCAttackRange();
    moveProjectiles();
    if(character->getPlayerSlash()->getIsSlashing()){
        character->getPlayerSlash()->checkCollide();
    }
}

void GameScene::moveProjectiles(){
    for(Projectile* projectile : listProjectiles){
        if(projectile){
            projectile->moveProjectile();
        }
    }
}

void GameScene::removeProjectile(Projectile* projectile){
    if(projectile){
        listProjectiles.removeAll(projectile);
        delete projectile;
        projectile = nullptr;
    }
}

void GameScene::removeEntity(Entity* entity){
    if(entity){
        listNPC.removeAll(entity);
        delete entity;
        entity = nullptr;
    }
}



void GameScene::checkNPCAttackRange(){
    qreal posX = character->getCenterPosition().x();
    qreal posY = character->getCenterPosition().y();

    if(character->isEntityDead()) return; //Do not attack if player is dead

    for(Entity* entity : listNPC){
        if(entity &&  entity->getHp() > 0){
            float distance = sqrt(pow(posX - entity->getCenterPosition().x(), 2) + pow(posY - entity->getCenterPosition().y(), 2));
            if(distance <= entity->getRangeAttack() + 1 && !(entity->isAttacking())){
                entity->attackEntity(character);
            }
        }
    }
}

//Move the player
void GameScene::movePlayer(){
    if(character->isEntityDead()) return; //Do not move the player if he is dead
    qreal* deltaPosition = getDeltaPosition();
    character->moveEntityCollision(deltaPosition[0], deltaPosition[1]);
    mainView->centerOn(character);
    delete[] deltaPosition;
}

//Move all entities
void GameScene::moveNPC(){
    if(character->isEntityDead()) return; //Do not move entities if the player is Dead

    //Get player position
    qreal posCharacterX = character->getCenterPosition().x();
    qreal posCharacterY = character->getCenterPosition().y();

    //We move each entity in listNPC
    for(Entity* entity : listNPC){
        float distance = sqrt(pow(posCharacterX - entity->getCenterPosition().x(), 2) + pow(posCharacterY - entity->getCenterPosition().y(), 2));
        if(entity){
            if(distance < 300){
                entity->moveEntity(posCharacterX, posCharacterY);
            } else{
                entity->idleAnimation();
            }
        }
    }
}

qreal* GameScene::getDeltaPosition() {
    qreal dx = 0;
    qreal dy = 0;

    int speed = character->getSpeed();

    if (!activeKeys.isEmpty()) {
        for (int key : activeKeys) {
            if (key == Qt::Key_Up || key == Qt::Key_Z){
                dy -= speed;
            }
            else if (key == Qt::Key_Down || key == Qt::Key_S){
                dy += speed;
            }
            else if (key == Qt::Key_Left || key == Qt::Key_Q){
                dx -= speed;
            }
            else if (key == Qt::Key_Right || key == Qt::Key_D){
                dx += speed;
            }
        }

        // Normalisation for the diagonal movement
        if (dx != 0 && dy != 0) {
            qreal norm = sqrt(dx * dx + dy * dy);
            dx = dx / norm * speed;
            dy = dy / norm * speed;
        }

        if (fabs(dx) > fabs(dy)) {
            //Horizontal movement
            if (dx > 0 && character->getCurrentDirection() != Right) {
                //Right movement
                character->rightWalkAnimation();
                character->setCurrentDirection(Right);
            }
            else if (dx < 0 && character->getCurrentDirection() != Left) {
                //Left movement
                character->leftWalkAnimation();
                character->setCurrentDirection(Left);
            }
        } else if (fabs(dy) >= fabs(dx)) {
            //Vertical movement
            if (dy > 0 && character->getCurrentDirection() != Down) {
                //Down movement
                character->frontWalkAnimation();
                character->setCurrentDirection(Down);
            }
            else if (dy < 0 && character->getCurrentDirection() != Up) {
                //Up movement
                character->backWalkAnimation();
                character->setCurrentDirection(Up);
            }
        }
    }

    qreal* deltaPosition = new qreal[2];
    deltaPosition[0] = std::ceil(dx);
    deltaPosition[1] = std::ceil(dy);
    return deltaPosition;
}

GameScene::~GameScene(){
    delete mapItem;
    mapItem = nullptr;
    delete character;
    character = nullptr;
    for(Entity* entity : listNPC){
        delete entity;
        entity = nullptr;
    }
    listNPC.clear();
}


//Detection des clics

void GameScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QPointF clickPos = event->scenePos();
    QPointF playerPos = character->pos();

    //Check if the player is on the missile spell
    if (hud->getSpellWidget()->getSelectedSpell()[0]) {
        if (event->button() == Qt::LeftButton) {
            if (character->canShoot(clickPos) && hud->getSpellWidget()->getCurrentMissile() != 0) {
                character->shootProjectile(clickPos, this);
                this->hud->getSpellWidget()->shootedMissile();
            }
        }
        if (event->button() == Qt::RightButton) {
            if (character->canShoot(clickPos) && hud->getSpellWidget()->getCurrentMissile() > 2) {

                QPointF clickPosHigh = clickPos;
                QPointF clickPosLow = clickPos;
                switch (character->getCurrentDirection()) {
                    case Up:
                        clickPosHigh.setX(clickPos.x() - 50);
                        clickPosLow.setX(clickPos.x() + 50);
                        break;
                    case Down:
                        clickPosHigh.setX(clickPos.x() - 50);
                        clickPosLow.setX(clickPos.x() + 50);
                        break;
                    case Left:
                        clickPosHigh.setY(clickPos.y() - 50);
                        clickPosLow.setY(clickPos.y() + 50);
                        break;
                    case Right:
                        clickPosHigh.setY(clickPos.y() - 50);
                        clickPosLow.setY(clickPos.y() + 50);
                        break;
                    default:
                        return;
                }
                character->shootProjectile(clickPos, this);
                character->shootProjectile(clickPosHigh, this);
                character->shootProjectile(clickPosLow, this);
                this->hud->getSpellWidget()->shootedMissile();
                this->hud->getSpellWidget()->shootedMissile();
                this->hud->getSpellWidget()->shootedMissile();
            }
        }
    }
    else if (hud->getSpellWidget()->getSelectedSpell()[1]) {
        if (event->button() == Qt::LeftButton) {
            if (character->canShoot(clickPos)) {
                character->slashAttack(clickPos, this);
            }
        }
    }
    else if (hud->getSpellWidget()->getSelectedSpell()[2]){
        if(!hud->getSpellWidget()->getIsShieldOnCd()){
            this->character->getPlayerShield()->activeShield();
            hud->getSpellWidget()->shieldUsed();
        }
    }
}
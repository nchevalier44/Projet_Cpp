#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QException>
#include <QGraphicsSceneMouseEvent>
#include "GameScene.h"
#include "MainWindow.h"
#include "../entities/player/Player.h"



GameScene::GameScene(MainView* view, QObject* parent) : QGraphicsScene(parent), mainView(view){

    try{
        loadMap();
    } catch(QException e){
        qCritical() << "Error when loading the map : " << e.what();
    } catch(std::exception e){
        qCritical() << "Error when loading the map : " << e.what();
    }

    this->setSceneRect(0, 0, backgroundWidth, backgroundHeight);

    //Setting up the player's character
    this->character = new Player("Character", 3);
    this->character->setPos(400, 200);
    this->character->setSpeed(4);
    this->character->setScale(0.1);
    this->character->setFocus();
    this->mainView->setFocus(); //Set the focus on the mainView so we can detect the key press

    this->addItem(character);
    this->character->setMainView(mainView);

    //Load slash animation
    PlayerSlash* slash = new PlayerSlash(this);
    this->character->setPlayerSlash(slash);

    Bat* bat = new Bat("Bat", 1, this);
    bat->setPos(200, 200);
    this->addItem(bat);
    listNPC.append(bat);


    //Starting the timer to update the animation and mouvement
    this->timer = new QTimer(this);
    connect(this->timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
    this->timer->start(30); //every 30 milliseconds

}

void GameScene::loadMap(){

    //Load and parse json file
    QFile file("../assets/maps/testmap.json");
    file.open(QIODevice::ReadOnly);

    QJsonDocument document = QJsonDocument::fromJson(file.readAll());
    QJsonObject mapObject = document.object();

    //listPixmap will contains every tiles
    QMap<int, QPixmap> listPixmap;

    int tileWidth = mapObject["tilewidth"].toInt();
    int tileHeight = mapObject["tileheight"].toInt();
    int numberTileWidth = mapObject["width"].toInt();
    int numberTileHeight = mapObject["height"].toInt();

    this->backgroundWidth = numberTileWidth*tileWidth;
    this->backgroundHeight = numberTileHeight*tileHeight;

    QPixmap mapPixmap(backgroundWidth, backgroundHeight);
    QPainter painter(&mapPixmap);

    //First we get every tiles to add it into listPixmap
    QJsonArray tilesets = mapObject["tilesets"].toArray();
    for(QJsonValue tilesetValue : tilesets){
        QJsonObject tileset = tilesetValue.toObject();

        int firstGid = tileset["firstgid"].toInt();
        QString source = tileset["source"].toString();
        source = source.replace("tilesets", "texture");
        source = source.replace(".tsx", ".png");
        QPixmap tilesetImage(source); //load image

        int numColumns = tilesetImage.width() / tileWidth;
        int numRows = tilesetImage.height() / tileHeight;

        for (int row = 0; row < numRows; ++row) {
            for (int column = 0; column < numColumns; ++column) {
                int tileID = firstGid + (row * numColumns) + column;

                //We add one by one tiles so we 'cut' the image everytime
                QRect tileRect(column * tileWidth, row * tileHeight, tileWidth, tileHeight);
                QPixmap tilePixmap = tilesetImage.copy(tileRect);
                listPixmap[tileID] = tilePixmap;
            }
        }
    }

    QJsonArray layers = mapObject["layers"].toArray();
    for(QJsonValue layerValue : layers){
        QJsonObject layer = layerValue.toObject();

        if(layer["type"] == "tilelayer"){
            int width = layer["width"].toInt();
            int height = layer["height"].toInt();

            QJsonArray data = layer["data"].toArray();
            for(int y = 0; y < height; y++){ //line
                for(int x = 0; x < width; x++){ //column
                    int tileID = data[width * y + x].toInt();
                    if(tileID != 0){
                        painter.setOpacity(layer["opacity"].toDouble());
                        painter.drawPixmap(x * 32, y * 32, listPixmap[tileID]); //Draw the tile at the right position
                        painter.setOpacity(1);
                    }
                }
            }

            //Add collisions objects
        } else if(layer["type"] == "objectgroup" && layer["name"] == "collisions"){
            QJsonArray objects = layer["objects"].toArray();

            for(QJsonValue objectValue : objects){
                QJsonObject object = objectValue.toObject();

                int x = object["x"].toInt();
                int y = object["y"].toInt();
                int width = object["width"].toInt();
                int height = object["height"].toInt();
                bool isEllipse = object.contains("ellipse") && object["ellipse"].toBool();

                if (isEllipse) {
                    QGraphicsEllipseItem* ellipse = new QGraphicsEllipseItem(x, y, width, height, mapItem);
                    ellipse->setBrush(Qt::red);
                    ellipse->setPen(Qt::NoPen);
                    ellipse->setData(0, "collision");
                    ellipse->setZValue(100);
                    this->addItem(ellipse);
                } else {
                    QGraphicsRectItem* rect = new QGraphicsRectItem(x, y, width, height, mapItem);
                    rect->setBrush(Qt::red);
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
    if(character->isPlayerDead()) return;
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
}

void GameScene::moveProjectiles(){
    for(Projectile* projectile : listProjectiles){
        if(projectile){
            projectile->moveProjectile();
        }
    }
}

void GameScene::removeProjectile(Projectile* projectile){
    listProjectiles.removeAll(projectile);
    delete projectile;
    projectile = nullptr;
}

void GameScene::removeEntity(Entity* entity){
    listNPC.removeAll(entity);
    delete entity;
    entity = nullptr;
}



void GameScene::checkNPCAttackRange(){
    qreal posX = character->getCenterPosition().x();
    qreal posY = character->getCenterPosition().y();

    for(Entity* entity : listNPC){
        if(entity){
            float distance = sqrt(pow(posX - entity->getCenterPosition().x(), 2) + pow(posY - entity->getCenterPosition().y(), 2));
            if(distance <= entity->getRangeAttack() + 1 && !(entity->isAttacking())){
                entity->attackEntity(character);
            }
        }
    }
}

//Move the player
void GameScene::movePlayer(){
    qreal* deltaPosition = getDeltaPosition();
    character->moveEntity(deltaPosition[0], deltaPosition[1]);
    delete[] deltaPosition;
}

//Move all entities
void GameScene::moveNPC(){
    //Get player position
    qreal posCharacterX = character->getCenterPosition().x();
    qreal posCharacterY = character->getCenterPosition().y();

    //We move each entity in listNPC
    for(Entity* entity : listNPC){
        if(entity){
            entity->moveEntity(posCharacterX, posCharacterY);
        }
    }
}

qreal* GameScene::getDeltaPosition(){
    qreal dx = 0;
    qreal dy = 0;

    int n = activeKeys.length();

    //Check the last key pressed (currently always pressed)

    if(n!=0) {
        int lastKey = activeKeys[n - 1];
        if (lastKey == Qt::Key_Up || lastKey == Qt::Key_Z) {
            if (character->getCurrentDirection() != Up) {
                character->backWalkAnimation();
                character->setCurrentDirection(Up);
            }
            dy -= character->getSpeed();
        } else if (lastKey == Qt::Key_Down || lastKey == Qt::Key_S) {
            if (character->getCurrentDirection() != Down) {
                character->frontWalkAnimation();
                character->setCurrentDirection(Down);
            }
            dy += character->getSpeed();
        } else if (lastKey == Qt::Key_Left || lastKey == Qt::Key_Q) {
            if (character->getCurrentDirection() != Left) {
                character->leftWalkAnimation();
                character->setCurrentDirection(Left);
            }
            dx -= character->getSpeed();
        } else if (lastKey == Qt::Key_Right || lastKey == Qt::Key_D) {
            if (character->getCurrentDirection() != Right) {
                character->rightWalkAnimation();
                character->setCurrentDirection(Right);
            }
            dx += character->getSpeed();
        }
    }
    qreal* deltaPosition = new qreal[2];
    deltaPosition[0] = dx;
    deltaPosition[1] = dy;
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
                        clickPosHigh.setX(clickPos.x() - 25);
                        clickPosLow.setX(clickPos.x() + 25);
                        break;
                    case Down:
                        clickPosHigh.setX(clickPos.x() - 25);
                        clickPosLow.setX(clickPos.x() + 25);
                        break;
                    case Left:
                        clickPosHigh.setY(clickPos.y() - 25);
                        clickPosLow.setY(clickPos.y() + 25);
                        break;
                    case Right:
                        clickPosHigh.setY(clickPos.y() - 25);
                        clickPosLow.setY(clickPos.y() + 25);
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
}
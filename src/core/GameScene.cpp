#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QException>
#include <QGraphicsSceneMouseEvent>
#include "GameScene.h"



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

    this->addItem(character);
    this->character->setMainView(mainView);

    Bat* bat = new Bat("Bat", 1);
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
                        /*QGraphicsPixmapItem* tile = new QGraphicsPixmapItem(listPixmap[tileID]);
                        tile->setPos(x * 32, y * 32);
                        tile->setOpacity(layer["opacity"].toDouble());*/
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
                    QGraphicsEllipseItem* ellipse = new QGraphicsEllipseItem(x, y, width, height);
                    ellipse->setBrush(Qt::red);
                    ellipse->setPen(Qt::NoPen);
                    ellipse->setData(0, "collision");
                    ellipse->setZValue(100);
                    this->addItem(ellipse);
                } else {
                    QGraphicsRectItem* rect = new QGraphicsRectItem(x, y, width, height);
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
    QGraphicsPixmapItem* mapItem = new QGraphicsPixmapItem(mapPixmap);
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

}

//Move the player
void GameScene::movePlayer(){
    qreal posX = character->pos().x();
    qreal posY = character->pos().y();

    qreal* deltaPosition = getDeltaPosition();
    qreal dx = deltaPosition[0];
    qreal dy = deltaPosition[1];
    delete[] deltaPosition;

    //Now we move the player considering eventual collision

    QList<QGraphicsItem*> collisions;
    int numberCollisions;
    int i;

    //Check if there is horizontal collision with an object
    character->setX(posX + dx);
    collisions = character->collidingItems();
    numberCollisions = collisions.count();
    i = 0;
    while(i < numberCollisions && collisions[i]->data(0) != "collision"){
        i++;
    }
    if(i != numberCollisions){
        character->setX(posX);
    }

    //Check if there is vertical collision with an object
    character->setY(posY + dy);
    collisions = character->collidingItems();
    numberCollisions = collisions.count();
    i = 0;
    while(i < numberCollisions && collisions[i]->data(0) != "collision"){
        i++;
    }
    if(i != numberCollisions){
        character->setY(posY);
    }
    mainView->centerOn(character);
}

//Move all entities
void GameScene::moveNPC(){
    //Get player position
    qreal posCharacterX = character->pos().x();
    qreal posCharacterY = character->pos().y();

    //We move each entity in listNPC
    for(Entity* entity : listNPC){
        qreal posEntityX = entity->pos().x();
        qreal posEntityY = entity->pos().y();;
        qreal dx = posCharacterX - posEntityX;
        qreal dy = posCharacterY - posEntityY;
        qreal entitySpeed = entity->getSpeed();
        if(dx < 0){
            posEntityX -= entitySpeed;
        } else if(dx > 0){
            posEntityX += entitySpeed;
        }
        if(dy < 0){
            posEntityY -= entitySpeed;
        } else if(dy > 0){
            posEntityY += entitySpeed;
        }
        entity->setPos(posEntityX, posEntityY);

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

}


//Detection des clics

void GameScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QPointF clickPos = event->scenePos();
    QPointF playerPos = character->pos();

    if(event->button() == Qt::LeftButton){

        if(character->canShoot(clickPos) && hud->getSpellWidget()->getCurrentMissile() != 0){
            character->shootProjectile(clickPos, this);
            this->hud->getSpellWidget()->shootedMissile();
        }
    }
    if(event->button() == Qt::RightButton){
        if(character->canShoot(clickPos)  && hud->getSpellWidget()->getCurrentMissile() > 2){

            QPointF clickPosHigh = clickPos;
            QPointF clickPosLow = clickPos;
            switch(character->getCurrentDirection()) {
                case Up: clickPosHigh.setX(clickPos.x() - 25); clickPosLow.setX(clickPos.x() + 25); break;
                case Down: clickPosHigh.setX(clickPos.x() - 25); clickPosLow.setX(clickPos.x() + 25); break;
                case Left: clickPosHigh.setY(clickPos.y() - 25); clickPosLow.setY(clickPos.y() + 25); break;
                case Right: clickPosHigh.setY(clickPos.y() - 25); clickPosLow.setY(clickPos.y() + 25);break;
                default: return;
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


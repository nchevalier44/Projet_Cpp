#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QException>
#include "GameScene.h"
#include "HUD.h"

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
    this->character->setScale(0.15);

    this->addItem(character);
    this->character->setMainView(mainView);

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
                        QGraphicsPixmapItem* tile = new QGraphicsPixmapItem(listPixmap[tileID]);
                        tile->setPos(x * 32, y * 32);
                        tile->setOpacity(layer["opacity"].toDouble());
                        this->addItem(tile);//draw the tile at the right position
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

qreal* GameScene::getDeltaPosition(){
    qreal dx = 0;
    qreal dy = 0;

    int n = activeKeys.length();

    //Check the last key pressed (currently always pressed)
    if(n==0){
        currentDirection = None;
    }
    else {
        int lastKey = activeKeys[n - 1];
        if (lastKey == Qt::Key_Up || lastKey == Qt::Key_Z) {
            if (currentDirection != Up) {
                character->backWalkAnimation();
                currentDirection = Up;
            }
            dy -= character->getSpeed();
        } else if (lastKey == Qt::Key_Down || lastKey == Qt::Key_S) {
            if (currentDirection != Down) {
                character->frontWalkAnimation();
                currentDirection = Down;
            }
            dy += character->getSpeed();
        } else if (lastKey == Qt::Key_Left || lastKey == Qt::Key_Q) {
            if (currentDirection != Left) {
                character->leftWalkAnimation();
                currentDirection = Left;
            }
            dx -= character->getSpeed();
        } else if (lastKey == Qt::Key_Right || lastKey == Qt::Key_D) {
            if (currentDirection != Right) {
                character->rightWalkAnimation();
                currentDirection = Right;
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
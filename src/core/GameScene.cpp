#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QException>
#include "GameScene.h"
#include "HUD.h"

GameScene::GameScene(MainView* view, QObject* parent) : QGraphicsScene(parent), mainView(view){

    this->setSceneRect(0, 0, backgroundWidth, backgroundHeight);

    try{
        loadMap();
    } catch(QException e){
        qCritical() << "Error when loading the map : " << e.what();
    } catch(std::exception e){
        qCritical() << "Error when loading the map : " << e.what();
    }

    //Setting up the player's character
    this->character = new Player("Character", 3);
    this->character->setPos(400, 300);
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
                    ellipse->setPen(Qt::NoPen);
                    ellipse->setData(0, "collision");
                    ellipse->setZValue(100);
                    this->addItem(ellipse);
                } else {
                    QGraphicsRectItem* rect = new QGraphicsRectItem(x, y, width, height);
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

    QPointF pos = character->pos();
    qreal posX = pos.rx();
    qreal posY = pos.ry();

    int n = activeKeys.length();

    //Check the last key pressed (currently always pressed)
    if(n==0){
        currentDirection = None;
    }
    else{
        int lastKey = activeKeys[n-1];
        if(lastKey == Qt::Key_Up || lastKey == Qt::Key_Z){
            if(currentDirection != Up){
                character->backWalkAnimation();
                currentDirection = Up;
            }
            posY -= character->getSpeed();
        }
        else if(lastKey == Qt::Key_Down || lastKey == Qt::Key_S){
            if(currentDirection != Down){
                character->frontWalkAnimation();
                currentDirection = Down;
            }
            posY += character->getSpeed();
        }
        else if(lastKey == Qt::Key_Left || lastKey == Qt::Key_Q){
            if(currentDirection != Left){
                character->leftWalkAnimation();
                currentDirection = Left;
            }
            posX -= character->getSpeed();
        }
        else if(lastKey == Qt::Key_Right || lastKey == Qt::Key_D){
            if(currentDirection != Right){
                character->rightWalkAnimation();
                currentDirection = Right;
            }
            posX += character->getSpeed();
        }
    }

    //Check if there is a collision with an object
    QList<QGraphicsItem*> collisions = character->collidingItems();
    for (QGraphicsItem* item : collisions) {
        if (item->data(0) == "collision") { //We stop the player
            posX = pos.rx();
            posY = pos.ry();
            if(currentDirection == Up){
                posY -= character->getSpeed();
            } else if(currentDirection == Down){
                posY += character->getSpeed();
            } else if(currentDirection == Left){
                posX += character->getSpeed();
            } else if(currentDirection == Right){
                posX -= character->getSpeed();
            }
        }
    }


    character->setPos(posX, posY);
    mainView->centerOn(character);

}

GameScene::~GameScene(){

}
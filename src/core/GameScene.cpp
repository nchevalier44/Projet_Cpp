#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QException>
#include <QGraphicsSceneMouseEvent>
#include "GameScene.h"
#include "MainWindow.h"

//Constructor
GameScene::GameScene(AudioManager* audioManager, MainView* view, ScoreManager* scoreManager, QObject* parent) : QGraphicsScene(parent), audioManager(audioManager), scoreManager(scoreManager), mainView(view){
    //Add background music
    audioPlayer = new QMediaPlayer(this);
    QAudioOutput* audioOutput = new QAudioOutput(this);
    audioOutput->setVolume(0.35);
    audioPlayer->setAudioOutput(audioOutput);

    //Play the music when it's loaded
    connect(audioPlayer, &QMediaPlayer::mediaStatusChanged, audioPlayer, [=]() {
        if (audioPlayer->mediaStatus() == QMediaPlayer::LoadedMedia) {
            audioPlayer->play();
        }
    });
    audioPlayer->setSource(QUrl::fromLocalFile(PATH_GAME_MUSIC));
    audioPlayer->setLoops(QMediaPlayer::Infinite);
    audioManager->addMusicObject(audioOutput, audioOutput->volume());


    this->setSceneRect(0, 0, backgroundWidth, backgroundHeight);


    //Setting up the player's character
    this->character = new Player("Character", 3, scoreManager, this);
    this->character->setPos(1480, 2730);
    this->character->setSpeed(6);
    this->character->setScale(0.2);
    this->character->setZValue(40);

    this->addItem(character);
    this->character->setMainView(mainView);

    //Load slash animation
    PlayerSlash* slash = new PlayerSlash(this, character, character);
    this->character->setPlayerSlash(slash);

    //Load the shield animation
    PlayerShield* shield = new PlayerShield(this, character, character);
    this->character->setPlayerShield(shield);

    //Load map and ennemies
    loadOverworld();


    //Starting the timer to update the animation and mouvement
    this->timer = new QTimer();
    connect(this->timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
    this->timer->start(30); //every 30 milliseconds
    timerList.append(timer);
}


///////////MAPS METHODS/////////////////////
void GameScene::loadMap(QString mapPath, int mapWidth, int mapHeight){
    //Delete map if already exist
    if(mapItem != nullptr){
        delete mapItem;
        mapItem = nullptr;
        if(!listBackground.isEmpty()) {
            for(QGraphicsPixmapItem* item : listBackground) {
                if(item){
                    this->removeItem(item);
                    delete item;
                    item = nullptr;
                }
            }
            listBackground.clear();
        }
        for(Entity * entity : listNPC) {
            this->removeEntity(entity);
        }
        QStringList tags = {"collision", "missileSpellZone", "shieldSpellZone", "slashSpellZone", "chest", "DonjonEntryZone","exitDonjon","TreeHeart"};
        clearInteractionZones(tags);

    }

    //Load and parse json file
    QFile file(mapPath);
    file.open(QIODevice::ReadOnly);

    QJsonDocument document = QJsonDocument::fromJson(file.readAll());
    QJsonObject mapObject = document.object();
    this->backgroundWidth = mapWidth;
    this->backgroundHeight = mapHeight;

    QPixmap mapPixmap(backgroundWidth, backgroundHeight);
    QPainter painter(&mapPixmap);

    //For each layer, print images on the pixmap
    QJsonArray layers = mapObject["layers"].toArray();
    for(QJsonValue layerValue : layers) {
        QJsonObject layer = layerValue.toObject();

        if (layer["type"] == "imagelayer") {
            QString imageName = layer["image"].toString();
            QPixmap image("../assets/maps/" + imageName);
            if (image.isNull()) {
                qWarning() << "Image not found:" << imageName;
                continue;
            }

            if (imageName == "layers/treePassage.png" || imageName == "layers/bigTreeTop.png") {
                QGraphicsPixmapItem* item = new QGraphicsPixmapItem(image);
                listBackground.append(item);
                item->setZValue(50);
                item->setOpacity(layer["opacity"].toDouble());
                item->setPos(layer["x"].toDouble(), layer["y"].toDouble());
                this->addItem(item);
                continue;  // Go to next layer without draw
            }

            painter.setOpacity(layer["opacity"].toDouble());
            painter.drawPixmap(layer["x"].toInt(), layer["y"].toInt(), image);

        //Add collisions and interactions objects
        } else if (layer["name"] == "collisions") {
            addInteractionZone("collision", layer);
        } else if (layer["name"] == "missileSpellZone") {
            addInteractionZone( "missileSpellZone", layer);
        } else if (layer["name"] == "shieldSpellZone") {
            addInteractionZone("shieldSpellZone", layer);
        } else if (layer["name"] == "slashSpellZone") {
            addInteractionZone("slashSpellZone", layer);
        } else if (layer["name"] == "chest") {
            addInteractionZone("chest", layer);
        }else if (layer["name"] == "DonjonEntryZone") {
            addInteractionZone("DonjonEntryZone", layer);
        }else if(layer["name"] == "exitDonjon"){
            addInteractionZone("exitDonjon", layer);
        }else if(layer["name"] == "TreeHeart"){
            addInteractionZone("TreeHeart", layer);
        }
    }
    painter.end();
    mapItem = new QGraphicsPixmapItem(mapPixmap);
    this->addItem(mapItem); //Add the map
    file.close();
}

//Add an interaction zone
void GameScene::addInteractionZone(QString name, QJsonObject layer){
    QJsonArray objects = layer["objects"].toArray();

    for (QJsonValue objectValue: objects) {
        QJsonObject object = objectValue.toObject();

        int x = object["x"].toDouble();
        int y = object["y"].toDouble();
        int width = object["width"].toDouble();
        int height = object["height"].toDouble();


        if (object.contains("polygon") || object.contains("polyline")) {
            QPolygonF polygon;

            QJsonArray points;
            if (object.contains("polygon")) {
                points = object["polygon"].toArray();
            } else {
                points = object["polyline"].toArray();
            }

            for (const QJsonValue &pointValue: points) {
                QJsonObject point = pointValue.toObject();
                qreal px = point["x"].toDouble();
                qreal py = point["y"].toDouble();
                polygon << QPointF(x + px, y + py);
            }

            QGraphicsPolygonItem *polyItem = new QGraphicsPolygonItem(polygon, mapItem);
            polyItem->setPen(Qt::NoPen);
            polyItem->setData(0, name);
            polyItem->setZValue(100);
            this->addItem(polyItem);
        } else {
            QGraphicsRectItem *rect = new QGraphicsRectItem(x, y, width, height, mapItem);
            rect->setPen(Qt::NoPen);
            rect->setData(0,name);
            rect->setZValue(100);
            this->addItem(rect);
        }
    }
}

//Clear all interaction zones
void GameScene::clearInteractionZones(const QStringList& tags) {
    for (QGraphicsItem* item : this->items()) {
        QString tag = item->data(0).toString();
        if (tags.contains(tag)) {
            this->removeItem(item);
            delete item;
        }
    }
}

//Load the overworld map and ennemies
void GameScene::loadOverworld() {
    //Delete audioPlayer if exist
    if(audioPlayer) {
        audioPlayer->stop();
        delete audioPlayer;
        audioPlayer = nullptr;
    }

    //Create an audio player with another music
    audioPlayer = new QMediaPlayer(this);
    QAudioOutput* audioOutput = new QAudioOutput(this);
    audioOutput->setVolume(0.4);
    audioPlayer->setAudioOutput(audioOutput);

    //Play the music when it's loaded
    connect(audioPlayer, &QMediaPlayer::mediaStatusChanged, audioPlayer, [=]() {
        if (audioPlayer->mediaStatus() == QMediaPlayer::LoadedMedia) {
            audioPlayer->play();
        }
    });
    audioPlayer->setSource(QUrl::fromLocalFile(PATH_GAME_MUSIC));
    audioPlayer->setLoops(QMediaPlayer::Infinite);
    audioManager->addMusicObject(audioOutput, audioOutput->volume());

    //Loading the map
    try{
        loadMap("../assets/maps/map.json", 3000,3000);
    } catch(QException e){
        qCritical() << "Error when loading the map : " << e.what();
    } catch(std::exception e){
        qCritical() << "Error when loading the map : " << e.what();
    }

    //Adding ennemies
    QList<QPoint> enemyPositions = {  //Position for every ennemies
            {1762,1965}, {1947,1974}, {2197,2082}, {2583,1820},
            {2432,1477}, {2363,859}, {2189,839}, {2230,1011}, {1355,346},
            {1724,409}, {1584,395}, {1403,506}, {1667,41}, {1387,745},
            {798,917}, {776,1082}, {418,1107}, {502,938}, {384,1452},
            {572,1630}, {836,2111}, {1096,1988}, {249,2641}, {280,2815}, {477,2720}
    };

    //For each ennemies add it in the map
    for (int i = 0; i < enemyPositions.size(); ++i) {
        QPoint pos = enemyPositions[i];
        if (i % 2 == 0) {
            Goblin* goblin = new Goblin("Goblin", GOBLIN_HP, scoreManager, this);
            goblin->setPos(pos);
            this->addItem(goblin);
            listNPC.append(goblin);
        } else {
            Bat* bat = new Bat("Bat", BAT_HP, scoreManager, this);
            bat->setPos(pos);
            this->addItem(bat);
            listNPC.append(bat);
        }
    }
}

//Load the dungeon map and the boss
void GameScene::loadDungeon() {
    try{ //Loading the map
        loadMap("../assets/maps/mapDonjon.json", 2000,2000);
    } catch(QException e){
        qCritical() << "Error when loading the map : " << e.what();
    } catch(std::exception e){
        qCritical() << "Error when loading the map : " << e.what();
    }

    //teleport player in the dungeon
    character->setPos(990,1350);

    //If he has tree heart, he already defeated the boss
    if(!character->getHasTreeHeart()){

        //Delete audioPlayer if exist
        if(audioPlayer) {
            audioPlayer->stop();
            delete audioPlayer;
            audioPlayer = nullptr;
        }

        //Create an audio player with another music
        audioPlayer = new QMediaPlayer(this);
        QAudioOutput* audioOutput = new QAudioOutput(this);
        audioOutput->setVolume(0.2);
        audioPlayer->setAudioOutput(audioOutput);

        //Play the music when it's loaded
        connect(audioPlayer, &QMediaPlayer::mediaStatusChanged, audioPlayer, [=]() {
            if (audioPlayer->mediaStatus() == QMediaPlayer::LoadedMedia) {
                audioPlayer->play();
            }
        });
        audioPlayer->setSource(QUrl::fromLocalFile(PATH_GAME_MUSIC2));
        audioPlayer->setLoops(QMediaPlayer::Infinite);
        audioManager->addMusicObject(audioOutput, audioOutput->volume());


        //Load the boss
        CrystalKnight* crystalKnight = new CrystalKnight("CrystalKnight", 150,character, scoreManager, this);
        crystalKnight->setPos(1000, 880);
        this->addItem(crystalKnight);
        listNPC.append(crystalKnight);
    }



}


//Mouvement functions
//Adapt the animation according to the direction
void GameScene::keyPressEvent(QKeyEvent* event){
    if(isPlayerDead || isPaused) return;

    if(event->isAutoRepeat()){
        return; //They key stay pressed so the walk animation can continue
    }

    if(!character->isBeenTakingKnockback()){
        activeKeys.append(event->key());
    }


    switch (event->key()){
        case Qt::Key_A :
            hud->getSpellWidget()->changeSelectedSpell(0);
            break;
        case Qt::Key_W :
            hud->getSpellWidget()->changeSelectedSpell(1);
            break;
        case Qt::Key_X :
            hud->getSpellWidget()->changeSelectedSpell(2);
        case Qt::Key_Escape:
            putGamePaused();
            break;
    }
}

//Delete player
void GameScene::deletePlayer() {
    delete character;
    character = nullptr;
}

//Set game in pause
void GameScene::putGamePaused(){
    isPaused = true;

    //Pause the timers
    for(QTimer* t : timerList){
        if(t){
            if(t->isActive()){
                t->stop();
            }
        }
    }

    //Pause the movies
    for(QMovie* m : movieList){
        if(m) m->setPaused(true);
    }

    //Pause the HUD animations
    for(QLabel* anim : hud->getHPWidget()->getLifeList()){
        anim->movie()->setPaused(true);
    }

    //Set score with the right time played
    scoreManager->getActualScore()->setTimePlayed(scoreManager->getElapsedTimer()->elapsed() / 1000);
    scoreManager->getActualScore()->setDate(QDateTime::currentDateTime().toString("dd/MM/yyyy"));
    mainView->displayPauseMenu();
}

//Cancel time and animation stop
void GameScene::reverseGamePaused(){
    isPaused = false;

    //Pause the timers
    for(QTimer* t : timerList){
        if(t) t->start();
    }

    //Pause the movies
    for(QMovie* m : movieList){
        if(m) m->setPaused(false);
    }

    //Pause the HUD animations
    for(QLabel* anim : hud->getHPWidget()->getLifeList()){
        anim->movie()->setPaused(false);
    }

    this->mainView->setFocus(); //Set the focus on the mainView so we can detect the key press
}

//Set the idle animation according to the last key pressed
void GameScene::keyReleaseEvent(QKeyEvent *event) {
    if(isPlayerDead || event->isAutoRepeat()){
        return; //They key stay pressed so the walk animation can continue
    }

    activeKeys.removeAll(event->key());

    if(!activeKeys.isEmpty()) return; //We change the animation to idle only if there are no others key pressed

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

//Function called every 30 ms
void GameScene::timerUpdate(){
    if(isPlayerDead) return;
    moveNPC();
    movePlayer();
    checkNPCAttackRange(); //He can be deleted here if an ennemy attack him
    if(isPlayerDead) return;
    moveProjectiles();
    if(character->getPlayerSlash()->getIsSlashing()){
        character->getPlayerSlash()->checkCollide();
    }
}

//Move projectiles
void GameScene::moveProjectiles(){
    for(Projectile* projectile : listProjectiles){
        if(projectile){
            projectile->moveProjectile();
        }
    }
}

//Remove projectiles
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
        entity->deleteLater();
        entity = nullptr;
    }
}


//Monsters attack player if the player is in their range attack
void GameScene::checkNPCAttackRange(){
    if(isPlayerDead) return;

    qreal posX = character->getCenterPosition().x();
    qreal posY = character->getCenterPosition().y();

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
    if(isPlayerDead || character->isBeenTakingKnockback()) return; //Do not move the player if he is dead

    qreal* deltaPosition = getDeltaPosition();
    character->moveEntityCollision(deltaPosition[0], deltaPosition[1]);
    mainView->centerOn(character);
    checkInteractionZone();
    delete[] deltaPosition;
}

void GameScene::checkInteractionZone(){
    if(isPlayerDead) return;
    bool inValidZone = false;

    QList<QGraphicsItem*> collisions = this->collidingItems(character);
    for(int i = 0; i < collisions.size(); i++){
        QString interactionZoneName = collisions[i]->data(0).toString();
        if(interactionZoneName == "missileSpellZone") {
            inValidZone = true;
            if(!character->getHasMissile()){
                if(!tooltiptxt){
                    showTooltip(character->pos(), "Press F to interact");
                }
                for(int key : activeKeys){
                    if(key == Qt::Key_F){
                        powerUpSound();
                        removeTooltip();
                        character->setHasMissile(true);
                        hud->getSpellWidget()->getSpell()[0]->show();
                        hud->getHPWidget()->addHP();
                        character->setHp(character->getCurrentHP() + 1);
                        hud->getHPWidget()->setLife(character->getCurrentHP());
                        if(!tooltiptxt && !tooltiprect){
                            showTooltip(character->pos(), "Press A to select the spell");
                        }
                        //add a single shot delete
                        QTimer::singleShot(2000, [this](){
                            removeTooltip();
                        });
                        break;
                    }
                }
            }
        }
        else if(interactionZoneName == "shieldSpellZone"){
            inValidZone = true;
            if(!character->getHasShield()){
                if(!tooltiptxt){
                    showTooltip(character->pos(), "Press F to interact");
                }
                for(int key : activeKeys){
                    if(key == Qt::Key_F){
                        powerUpSound();
                        removeTooltip();
                        character->setHasShield(true);
                        hud->getSpellWidget()->getSpell()[2]->show();
                        hud->getHPWidget()->addHP();
                        character->setHp(character->getCurrentHP() + 1);
                        hud->getHPWidget()->setLife(character->getCurrentHP());
                        if(!tooltiptxt && !tooltiprect){
                            showTooltip(character->pos(), "Press X to select the spell");
                        }
                        //add a single shot delete
                        QTimer::singleShot(2000, [this](){
                            removeTooltip();
                        });
                        break;
                    }
                }
            }

        }
        else if(interactionZoneName == "slashSpellZone"){
            inValidZone = true;

            if(!character->getHasSlash()){
                if(!tooltiptxt){
                    showTooltip(character->pos(), "Press F to interact");
                }
                for(int key : activeKeys){
                    if(key == Qt::Key_F){
                        powerUpSound();
                        removeTooltip();
                        character->setHasSlash(true);
                        hud->getSpellWidget()->getSpell()[1]->show();
                        hud->getHPWidget()->addHP();
                        character->setHp(character->getCurrentHP() + 1);
                        hud->getHPWidget()->setLife(character->getCurrentHP());
                        if(!tooltiptxt && !tooltiprect){
                            showTooltip(character->pos(), "Press W to select the spell");
                        }
                        //add a single shot delete
                        QTimer::singleShot(2000, [this](){
                            removeTooltip();
                        });
                        break;
                    }
                }
            }

        }
        else if(interactionZoneName == "chest"){
            inValidZone = true;
            if(!tooltiptxt){
                showTooltip(character->pos(), "Press F to interact");
                for(int key : activeKeys){
                    if(key == Qt::Key_F){
                        removeTooltip();
                        break;
                    }
                }

            }
        }
        else if(interactionZoneName == "DonjonEntryZone") {
            inValidZone = true;
            if(!tooltiptxt){
                showTooltip(character->pos(), "Press F to enter");
            }
            for(int key : activeKeys){
                if(key == Qt::Key_F){
                    loadDungeon();
                    removeTooltip();
                    break;
                }
            }
        }

        else if(interactionZoneName == "exitDonjon"){
            inValidZone = true;
            if(!tooltiptxt){
                showTooltip(character->pos(), "Press F to exit dungeon");
            }
            for(int key : activeKeys){
                if(key == Qt::Key_F){
                    loadOverworld();
                    character->setPos(500, 2635); //Reset the player position
                    removeTooltip();
                    break;
                }
            }
        }
        else if(interactionZoneName == "TreeHeart"){
            inValidZone = true;
            if(character->getHasTreeHeart()){
                if(!tooltiptxt){
                    showTooltip(character->pos(), "Press F to replace the Tree Heart");
                }
                for(int key : activeKeys) {
                    if (key == Qt::Key_F) {
                        scoreManager->getActualScore()->setTimePlayed(scoreManager->getElapsedTimer()->elapsed() / 1000);
                        scoreManager->getActualScore()->setDate(QDateTime::currentDateTime().toString("dd/MM/yyyy"));
                        mainView->displayWinScreen();
                    }
                }
            }
        }
    }
    if(!inValidZone){
        removeTooltip();
    }
}

void GameScene::showTooltip(QPointF pos, QString text){
    if (tooltiptxt || tooltiprect) return;

    // Text
    tooltiptxt = new QGraphicsTextItem(text);
    QFont font("Cinzel Decorative", 12);
    tooltiptxt->setFont(font);
    tooltiptxt->setDefaultTextColor(QColor("#d6c7ff"));
    tooltiptxt->setZValue(100);

    // Size of text
    QFontMetrics metrics(font);
    QRect textRect = metrics.boundingRect(text);
    int padding = 5;

    // Background
    tooltiprect = new QGraphicsRectItem(textRect.adjusted(-padding, -padding, padding, padding));
    tooltiprect->setBrush(QColor(40, 30, 60, 180));
    tooltiprect->setPen(QPen(QColor(120, 100, 180), 2));
    tooltiprect->setZValue(90);  // derrière le texte

    //Position
    tooltiptxt->setPos(pos.x() -1, pos.y()  - 50);
    tooltiprect->setPos(pos.x() , pos.y() + textRect.height() - 50);

    // Glow effect
    QGraphicsDropShadowEffect* glow = new QGraphicsDropShadowEffect();
    glow->setColor(QColor(140, 115, 210));
    glow->setBlurRadius(15);
    glow->setOffset(0, 0);
    tooltiprect->setGraphicsEffect(glow);
    tooltiptxt->setGraphicsEffect(glow);

    this->addItem(tooltiptxt);
    this->addItem(tooltiprect);
}

void GameScene::removeTooltip(){
    if(tooltiptxt){
        this->removeItem(tooltiptxt);
        delete tooltiptxt;
        tooltiptxt = nullptr;
    }
    if(tooltiprect){
        this->removeItem(tooltiprect);
        delete tooltiprect;
        tooltiprect = nullptr;
    }
}

//Move all entities
void GameScene::moveNPC(){
    if(isPlayerDead) return; //Do not move entities if the player is Dead

    //Get player position
    qreal posCharacterX = character->getCenterPosition().x();
    qreal posCharacterY = character->getCenterPosition().y();

    //We move each entity in listNPC
    for(Entity* entity : listNPC){
        if(dynamic_cast<CrystalKnight*>(entity)) return; //Do not move the CrystalKnight, he teleports
        if(entity){
            float distance = sqrt(pow(posCharacterX - entity->getCenterPosition().x(), 2) + pow(posCharacterY - entity->getCenterPosition().y(), 2));
            if(distance < mainView->mapToScene(mainView->viewport()->rect()).boundingRect().width() * 0.5){
                entity->moveEntity(posCharacterX, posCharacterY);
                entity->updateFlipFromPlayerPosition(character->getCenterPosition()); //Flip entities in the right direction (because their animation are in one side)
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

//Destructor
GameScene::~GameScene(){
    if(mapItem){
        delete mapItem;
        mapItem = nullptr;
    }
    if(character){
        delete character;
        character = nullptr;
    }
    if(timer){
        delete timer;
        timer = nullptr;
    }
    for(Entity* entity : listNPC){
        delete entity;
        entity = nullptr;
    }
    listNPC.clear();
}


//Detection of clicks
void GameScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if(isPlayerDead || isPaused) return;
    QPointF clickPos = event->scenePos();

    //Check if the player is on the missile spell
    if (hud->getSpellWidget()->getSelectedSpell()[0] && character->getHasMissile()) {
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
    else if (hud->getSpellWidget()->getSelectedSpell()[1] && character->getHasSlash()) {
        if (event->button() == Qt::LeftButton) {
            if (character->canShoot(clickPos)) {
                character->slashAttack(clickPos, this);
            }
        }
    }
    else if (hud->getSpellWidget()->getSelectedSpell()[2] && character->getHasShield()) {
        if(!hud->getSpellWidget()->getIsShieldOnCd()){
            this->character->useShield();
            hud->getSpellWidget()->shieldUsed();
        }
    }
}

//Sound of power up (when we get spells)
void GameScene::powerUpSound(){
    QSoundEffect* powerUpSFX = new QSoundEffect();
    connect(powerUpSFX, &QSoundEffect::loadedChanged, powerUpSFX, &QSoundEffect::play);
    powerUpSFX->setSource(QUrl::fromLocalFile(PATH_PLAYER_POWER_UP_SOUND));
    powerUpSFX->setVolume(0.45);
    audioManager->addSFXObject(powerUpSFX, powerUpSFX->volume());
    connect(powerUpSFX, &QSoundEffect::playingChanged, [powerUpSFX](){
        if(!powerUpSFX->isPlaying()){
            delete powerUpSFX;
        }
    });
}
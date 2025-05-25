#include <QTimer>
#include "../../core/GameScene.h"
#include "CrystalKnight.h"



CrystalKnight::CrystalKnight(std::string name, int life, Player* player, ScoreManager* scoreManager, GameScene* scene, QGraphicsItem* parent) : Entity(name, life, scoreManager, scene, parent), player(player) {
    setAnimation(PATH_CK_IDLE, NB_FRAME_CK_IDLE, ANIM_SPEED_CK_IDLE);
    this->setScale(0.25);
    maxHp = life;
    speed = 2;
    score = 100;
    QRectF baseZone(750, 690, 550, 430); //Arena zone
    QRectF spriteRect = this->boundingRect();
    QSizeF spriteSize = spriteRect.size() * this->scale();

    //Zone where the boss can teleport
    zoneTP = QRectF(
            baseZone.left()- spriteSize.width()/2,
            baseZone.top()- spriteSize.height()/2,
            baseZone.width() - spriteSize.width(),
            baseZone.height() - spriteSize.height()
    );
    //Zone where the boss can attack
    zoneAttack = QRectF(
            baseZone.left()- 30,
            baseZone.top()- 100,
            baseZone.width() - 30,
            baseZone.height() - 100
    );
    attackLoop();

    pathHitSound = PATH_CK_HIT_SOUND;
    pathDeathSound = PATH_CK_DEATH_SOUND;


}

void CrystalKnight::attackLoop(){
    //Loop called so the boss attack automatically
    QTimer::singleShot(3000, this, [this]() {
        if(gameScene->isGamePaused()){
            QTimer::singleShot(3000, this, &CrystalKnight::attackLoop);
        } else if(!isDead && !player->isEntityDead()) {
            performRandomAction();
        }
    });
}

void CrystalKnight::performRandomAction(){
    //Choose a random number (1,2 or 3) and act according to it
    int randomAction = QRandomGenerator::global()->bounded(3);
    switch(randomAction){
        case 0:
            moveAnimation();
            break;
        case 1:
            lightningAttck();
            break;
        case 2 :
            teleportOnPlayer();
            break;
        default:
            break;
    }

}

void CrystalKnight::moveAnimation(qreal x , qreal y ) {
    setAnimation(PATH_CK_MOVE, NB_FRAME_CK_MOVE, ANIM_SPEED_CK_MOVE);
    //Move the boss during the invisible frames
    QTimer::singleShot(7*ANIM_SPEED_CK_MOVE, this, [this,x,y]() {
        teleport(x,y);
    });

    tpSound();

    //back to idle
    if(!isAttacking){
        QTimer::singleShot(NB_FRAME_CK_MOVE*ANIM_SPEED_CK_MOVE, this, [this]() {
            setAnimation(PATH_CK_IDLE, NB_FRAME_CK_IDLE, ANIM_SPEED_CK_IDLE);
        });
        attackLoop();
    }
}

void CrystalKnight::deathAnimation() {
    isDead = true;
    setPos(1000,850); //Placing at the center of the room
    player->setHasTreeHeart(true);
    gameScene->showTooltip(player->pos(),"You found the Tree Heart !");
    setAnimation(PATH_CK_DEATH, NB_FRAME_CK_DEATH, ANIM_SPEED_CK_DEATH);
    QTimer::singleShot(NB_FRAME_CK_DEATH*ANIM_SPEED_CK_DEATH, this, [this]() { //Removing the boss then
        stopAnimation();
        gameScene->removeEntity(this);
    });
}

void CrystalKnight::teleportOnPlayer() {
    //Attack where the boss teleports above the player and performs a claw attack
    if (player) {
        isAttacking = true;
        qreal playerX = player->x();
        qreal playerY = player->y();
        moveAnimation(playerX- frameWidth / 5, playerY - frameHeight /5);
        QTimer::singleShot(NB_FRAME_CK_MOVE*ANIM_SPEED_CK_MOVE, this, [this]() {
            clawAttackAnimation();
            attackLoop();
        });
    }
}

void CrystalKnight::teleport(qreal x, qreal y) {
    //Either he teleports to a random position either to the given position
    if (x != 0 && y != 0) {
        this->setPos(x, y);
        return;
    }
    //Call with quint32 to be used in bounded method
    quint32 minX = static_cast<quint32>(zoneTP.left());
    quint32 maxX = static_cast<quint32>(zoneTP.right());
    quint32 minY = static_cast<quint32>(zoneTP.top());
    quint32 maxY = static_cast<quint32>(zoneTP.bottom());

    qreal randX = QRandomGenerator::global()->bounded(minX, maxX + 1);
    qreal randY = QRandomGenerator::global()->bounded(minY, maxY + 1);
    this->setPos(randX, randY);
}


void CrystalKnight::lightningAttck(){
    //Perform lightnings attacks all over the arena
    lightningCount = 0;
    if (!lightningTimer) {
        lightningTimer = new QTimer(this);
        connect(lightningTimer, &QTimer::timeout, this, [this]() {
            if (lightningCount >= maxLightningCount) { //When reached the limit, continue on another loop
                lightningTimer->stop();
                attackLoop();
                return;
            }
            Lightning* lightning = new Lightning(zoneAttack,player, gameScene); //Create a new lightning every 100ms
            gameScene->addItem(lightning);
            lightningCount++;
        });
    }
    lightningTimer->start(100);
}

void CrystalKnight::clawAttackAnimation(){
    isClawAttacking = true;
    isAttacking = true;
    if (!clawTimer) {
        clawTimer = new QTimer(this);
        connect(clawTimer, &QTimer::timeout, this, &CrystalKnight::checkCollisionsClawAttack); //Check if the claw touched the player
    }
    clawTimer->start(30);
    setAnimation(PATH_CK_ATTACK_RIGHT, NB_FRAME_CK_ATTACK_LR, ANIM_SPEED_CK_ATTACK_LR);
    QTimer::singleShot(NB_FRAME_CK_ATTACK_LR*ANIM_SPEED_CK_ATTACK_LR, this, [this]() {
        setAnimation(PATH_CK_IDLE, NB_FRAME_CK_IDLE, ANIM_SPEED_CK_IDLE);
        isAttacking = false;
        isClawAttacking = false;
        clawAttackTouched = false;
        if (clawTimer) clawTimer->stop();
    });
}

void CrystalKnight::checkCollisionsClawAttack(){
    if(gameScene->isGamePaused()) return;
    QList<QGraphicsItem*> collisions = gameScene->collidingItems(this);
    int n = collisions.length();
    int i = 0;
    while(i < n){
        //dynamic_cast<Player*> return a Player if it's a player else nullptr
        Player* testPlayer = dynamic_cast<Player*>(collisions[i]); //Cast the player, nullptr if not a player
        if(testPlayer && isClawAttacking && !clawAttackTouched) {
            player->takeDamage(1, this);
            clawAttackTouched = true;
        }
        i++;
    }
}

void CrystalKnight::tpSound() {
    //Sound effect for teleportation
    QSoundEffect* tpSFX = new QSoundEffect();
    connect(tpSFX, &QSoundEffect::loadedChanged, tpSFX, &QSoundEffect::play);
    tpSFX->setSource(QUrl::fromLocalFile(PATH_CK_TP_SOUND));
    tpSFX->setVolume(0.35);
    gameScene->getAudioManager()->addSFXObject(tpSFX, tpSFX->volume());
    connect(tpSFX, &QSoundEffect::playingChanged, [tpSFX](){
        if(!tpSFX->isPlaying()){
            delete tpSFX;
        }
    });
}





//LIGHTNING METHODS

Lightning::Lightning(const QRectF& spawnZone, Player* player,GameScene* scene,QGraphicsItem* parent)
        : QGraphicsPixmapItem(parent), currentFrame(0), zone(spawnZone), gameScene(scene), player(player)
{
    spriteSheet = QPixmap(PATH_CK_THUNDER);
    if (spriteSheet.isNull()) {
        qWarning("Impossible de charger le sprite sheet de la foudre");
        return;
    }

    setRandomPosition();

    // Affiche la première frame
    QPixmap framePixmap = spriteSheet.copy(0, 0, frameWidth, frameHeight);
    setPixmap(framePixmap.scaled(frameWidth/3, frameHeight/3, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    // Setup timer d’animation
    connect(&timer, &QTimer::timeout, this, &Lightning::updateFrame);
    connect(&timer, &QTimer::timeout, this, &Lightning::checkCollisions);
    timer.start(animSpeedMs);
    gameScene->getTimerList().append(&timer);

    QTimer::singleShot(animSpeedMs*6, this, [=](){
        thunderSound();
    });
}

Lightning::~Lightning() {
    timer.stop();
    gameScene->getTimerList().removeAll(&timer);
}

void Lightning::thunderSound() {
    // Sound effect for thunder
    QSoundEffect* thunderSFX = new QSoundEffect();
    connect(thunderSFX, &QSoundEffect::loadedChanged, thunderSFX, &QSoundEffect::play);
    thunderSFX->setSource(QUrl::fromLocalFile(PATH_CK_THUNDER_SOUND));
    thunderSFX->setVolume(0.05);
    gameScene->getAudioManager()->addSFXObject(thunderSFX, thunderSFX->volume());
    connect(thunderSFX, &QSoundEffect::playingChanged, [thunderSFX](){
        if(!thunderSFX->isPlaying()){
            delete thunderSFX;
        }
    });
}

void Lightning::setRandomPosition() {
    //Set a random position within the defined zone
    quint32 minX = static_cast<quint32>(zone.left());
    quint32 maxX = static_cast<quint32>(zone.right());
    quint32 minY = static_cast<quint32>(zone.top());
    quint32 maxY = static_cast<quint32>(zone.bottom());

    qreal randX = QRandomGenerator::global()->bounded(minX, maxX + 1);
    qreal randY = QRandomGenerator::global()->bounded(minY, maxY + 1);
    this->setPos(randX, randY);
}

void Lightning::updateFrame() {
    currentFrame++;
    if (currentFrame >= frameCount) {
        // Animation terminée, suppression de l'objet
        timer.stop();
        scene()->removeItem(this);
        deleteLater();
        return;
    }

    // Extraire et afficher la frame suivante
    QPixmap framePixmap = spriteSheet.copy(currentFrame * frameWidth, 0, frameWidth, frameHeight);
    setPixmap(framePixmap.scaled(frameWidth/3, frameHeight/3, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}


QRectF Lightning::boundingRect() const {
    // boundingRect doit correspondre à la taille actuelle affichée
    qreal scaleFactor = 0.33;
    return QRectF(0, 0, frameWidth * scaleFactor, frameHeight * scaleFactor);
}

QPainterPath Lightning::shape() const {
    QPainterPath path;

    qreal scaleFactor = 0.33;
    QRectF fullRect(0, 0, frameWidth * scaleFactor, frameHeight * scaleFactor);

    //For the 6 to 9 frames, we add a hitbox at the bottom of the lightning
    if (currentFrame >= 6 && currentFrame <= 9) {
        qreal hitboxHeight = 150 * scaleFactor;   // hauteur du hitbox (ajustable)
        QRectF hitboxRect(0, fullRect.height() - hitboxHeight, fullRect.width(), hitboxHeight);
        path.addRect(hitboxRect);
    }
    return path;
}

void Lightning::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    QGraphicsPixmapItem::paint(painter, option, widget);
}


void Lightning::checkCollisions() {
    //Check collision with the player
    if(gameScene->isGamePaused()) return;
    QList<QGraphicsItem*> collisions = gameScene->collidingItems(this);
    int n = collisions.length();
    int i = 0;
    while(!hasCollided && i < n){
        //dynamic_cast<Player*> return a Player if it's a player else nullptr
        Player* testPlayer = dynamic_cast<Player*>(collisions[i]); //Cast the player, nullptr if not a player
        if(testPlayer) {
            hasCollided = true;
            if(player && !player->getPlayerShield()->isActive()){ //If no shield
                player->takeDamage(1, nullptr);
            }
            else if(player && player->getPlayerShield()->isActive()){ //if shield
                player->getPlayerShield()->decreaseHP();
            }
        }
        i++;
    }
}

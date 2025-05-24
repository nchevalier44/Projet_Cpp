#include <QTimer>
#include "../../core/GameScene.h"
#include "CrystalKnight.h"



CrystalKnight::CrystalKnight(std::string name, int life, Player* player, ScoreManager* scoreManager, GameScene* scene, QGraphicsItem* parent) : Entity(name, life, scoreManager, scene, parent), player(player) {
    setAnimation(PATH_CK_IDLE, NB_FRAME_CK_IDLE, ANIM_SPEED_CK_IDLE);
    this->setScale(0.25);
    maxHp = life;
    speed = 2;
    score = 100;
    QRectF baseZone(750, 690, 550, 430);
    QRectF spriteRect = this->boundingRect();
    QSizeF spriteSize = spriteRect.size() * this->scale();

    zoneTP = QRectF(
            baseZone.left()- spriteSize.width()/2,
            baseZone.top()- spriteSize.height()/2,
            baseZone.width() - spriteSize.width(),
            baseZone.height() - spriteSize.height()
    );
    zoneAttack = QRectF(
            baseZone.left()- 30,
            baseZone.top()- 100,
            baseZone.width() - 30,
            baseZone.height() - 100
    );
    attackLoop();


}

void CrystalKnight::attackLoop(){
    QTimer::singleShot(3000, this, [this]() {
        if(!isDead) {
            performRandomAction();
        }
    });
}

void CrystalKnight::performRandomAction(){
    int randomAction = QRandomGenerator::global()->bounded(3);
    qDebug() << "Random action: " << randomAction;
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
    QTimer::singleShot(7*ANIM_SPEED_CK_MOVE, this, [this,x,y]() {
        teleport(x,y);
    });

    if(!isAttacking){
        qDebug() << "after teleport";
        QTimer::singleShot(NB_FRAME_CK_MOVE*ANIM_SPEED_CK_MOVE, this, [this]() {
            setAnimation(PATH_CK_IDLE, NB_FRAME_CK_IDLE, ANIM_SPEED_CK_IDLE);
        });
        attackLoop();
    }
}

void CrystalKnight::deathAnimation() {
    isDead = true;
    setPos(1000,850);
    player->setHasTreeHearth(true);
    setAnimation(PATH_CK_DEATH, NB_FRAME_CK_DEATH, ANIM_SPEED_CK_DEATH);
    QTimer::singleShot(NB_FRAME_CK_DEATH*ANIM_SPEED_CK_DEATH, this, [this]() {
        stopAnimation();
        gameScene->removeEntity(this);
    });
}

void CrystalKnight::teleportOnPlayer() {
    if (player) {
        isAttacking = true;
        qreal playerX = player->x();
        qreal playerY = player->y();
        moveAnimation(playerX- frameWidth / 5, playerY - frameHeight /5);
        qDebug() << "Frame width:" << frameWidth << "Frame height: " << frameHeight;
        QTimer::singleShot(NB_FRAME_CK_MOVE*ANIM_SPEED_CK_MOVE, this, [this]() {
            clawAttackAnimation();
            attackLoop();
        });
    }
}

void CrystalKnight::teleport(qreal x, qreal y) {
    if (x != 0 && y != 0) {
        this->setPos(x, y);
        return;
    }
    quint32 minX = static_cast<quint32>(zoneTP.left());
    quint32 maxX = static_cast<quint32>(zoneTP.right());
    quint32 minY = static_cast<quint32>(zoneTP.top());
    quint32 maxY = static_cast<quint32>(zoneTP.bottom());

    qreal randX = QRandomGenerator::global()->bounded(minX, maxX + 1); // +1 si tu veux inclure max
    qreal randY = QRandomGenerator::global()->bounded(minY, maxY + 1);
    this->setPos(randX, randY);
}


void CrystalKnight::lightningAttck(){
    lightningCount = 0;
    if (!lightningTimer) {
        lightningTimer = new QTimer(this);
        connect(lightningTimer, &QTimer::timeout, this, [this]() {
            if (lightningCount >= maxLightningCount) {
                lightningTimer->stop();
                attackLoop();
                return;
            }
            Lightning* lightning = new Lightning(zoneAttack,player, gameScene);
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
        connect(clawTimer, &QTimer::timeout, this, &CrystalKnight::checkCollisionsClawAttack);
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
    QList<QGraphicsItem*> collisions = gameScene->collidingItems(this);
    int n = collisions.length();
    int i = 0;
    while(i < n){
        //dynamic_cast<Player*> return a Player if it's a player else nullptr
        Player* testPlayer = dynamic_cast<Player*>(collisions[i]);
        if(testPlayer && isClawAttacking && !clawAttackTouched) {
            player->takeDamage(1, this);
            clawAttackTouched = true;
        }
        i++;
    }
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
}

Lightning::~Lightning() {
    timer.stop();
}

void Lightning::setRandomPosition() {

    quint32 minX = static_cast<quint32>(zone.left());
    quint32 maxX = static_cast<quint32>(zone.right());
    quint32 minY = static_cast<quint32>(zone.top());
    quint32 maxY = static_cast<quint32>(zone.bottom());


    qDebug() << "Zone X:" << minX << "to" << maxX;
    qDebug() << "Zone Y:" << minY << "to" << maxY;

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

    // Pour frames 6 à 9 inclus, shape = petit rectangle en bas du sprite
    if (currentFrame >= 6 && currentFrame <= 9) {
        qreal hitboxHeight = 150 * scaleFactor;   // hauteur du hitbox (ajustable)
        QRectF hitboxRect(0, fullRect.height() - hitboxHeight, fullRect.width(), hitboxHeight);
        path.addRect(hitboxRect);
    }
    return path;
}

void Lightning::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    QGraphicsPixmapItem::paint(painter, option, widget);

    painter->setPen(QPen(Qt::blue, 2, Qt::DashLine));
    painter->setBrush(QColor(0, 0, 255, 0));
    painter->drawRect(boundingRect());

    painter->setPen(QPen(Qt::red, 2, Qt::SolidLine));
    painter->setBrush(QColor(255, 0, 0, 0));
    painter->drawPath(shape());
}


void Lightning::checkCollisions() {
    QList<QGraphicsItem*> collisions = gameScene->collidingItems(this);
    int n = collisions.length();
    int i = 0;
    while(!hasCollided && i < n){
        //dynamic_cast<Player*> return a Player if it's a player else nullptr
        Player* testPlayer = dynamic_cast<Player*>(collisions[i]);
        if(testPlayer) {
            hasCollided = true;
            player->takeDamage(1, nullptr);
        }
        i++;
    }
}

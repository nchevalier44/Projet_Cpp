#include "PlayerSlash.h"
#include "../../core/GameScene.h"


PlayerSlash::PlayerSlash(GameScene *scene, Player* player, QGraphicsObject* parent) : scene(scene), player(player), QGraphicsObject(parent)
{
    scene->removeItem(this);
    scene->addItem(this);
    // We load the slashAnimation
    QMovie *slash1 = new QMovie(PATH_PLAYER_SLASH1);
    QMovie *slash2 = new QMovie(PATH_PLAYER_SLASH2);
    QMovie *slash3 = new QMovie(PATH_PLAYER_SLASH3);


    animationTimer = new QTimer(this);
    scene->getTimerList().append(animationTimer);

    attackAnimation.push_back(slash1);
    attackAnimation.push_back(slash2);
    attackAnimation.push_back(slash3);

    for (QMovie *movie : attackAnimation)
    {
        movie->setSpeed(100);
        connect(animationTimer, &QTimer::timeout, this, [this, movie](){
            if(isSlashing){
                if (attackAnimation[currentAttackIndex] == movie) {
                    currentPixmap = movie->currentPixmap();
                    if (movie->currentFrameNumber() == movie->frameCount() - 1) {
                        this->setVisible(false);
                        movie->jumpToFrame(0);
                        isSlashing = false;
                        currentAttackIndex = (currentAttackIndex + 1) % attackAnimation.size();
                        combotimer.restart();
                        entityDamagedList.clear();
                    } else{
                        movie->jumpToNextFrame();
                        this->scene->update(this->sceneBoundingRect());
                        animationTimer->setInterval(movie->nextFrameDelay());
                    }
                }
            }
        });
    }
    animationTimer->start();
    if(scene->isGamePaused()){
        animationTimer->stop();
    }
    this->setScale(0.25);
}

void PlayerSlash::slashAttack(QPointF pos, QPointF playerPos, Direction CurrentDirection)
{
    if(isSlashing) return;
    isSlashing = true;
    // Calculate the angle to properly rotate the slash
    attackPosition = pos;
    QPointF direction = attackPosition - playerPos;
    this->rotationAngle = std::atan2(direction.y(), direction.x()) * 180 / M_PI;

    // We check if the combo is still valid
    if (combotimer.isValid() && combotimer.elapsed() > comboMaxDelay)
    {
        currentAttackIndex = 0;
    }

    // Positionnement de l'attaque
    qreal length = std::sqrt(direction.x() * direction.x() + direction.y() * direction.y());
    if (length > 0)
    {
        direction /= length;
    }

    // adjuste the position of the attack according to the current direction
    qreal distance = 20; // distance à laquelle l'effet est affiché
    QPointF finalPos = playerPos + direction * distance;
    qreal w = player->sceneBoundingRect().width();
    qreal h = player->sceneBoundingRect().height();
    switch (CurrentDirection)
    {
        case Up:
            finalPos.setY(finalPos.y() - h * 0.8);
            this->setZValue(39);
            break;
        case Down:
            finalPos.setY(finalPos.y() + h * 0.75);
            break;
        case Left:
            finalPos.setX(finalPos.x() - w * 0.75);
            break;
        case Right:
            finalPos.setX(finalPos.x() + w * 0.75);
            this->setZValue(42);
            break;
        default:
            break;
    }
    this->setPos(finalPos);
    this->setVisible(true);



    //Sound effect
    QSoundEffect* slashSFX = new QSoundEffect();
    connect(slashSFX, &QSoundEffect::loadedChanged, slashSFX, &QSoundEffect::play);
    if(currentAttackIndex == 0 || currentAttackIndex == 1){
        slashSFX->setSource(QUrl::fromLocalFile(PATH_PLAYER_SLASH_SOUND));
    } else{
        slashSFX->setSource(QUrl::fromLocalFile(PATH_PLAYER_LAST_SLASH_SOUND));
    }

    slashSFX->setVolume(0.4);
    scene->getAudioManager()->addSFXObject(slashSFX, slashSFX->volume());
    connect(slashSFX, &QSoundEffect::playingChanged, [slashSFX](){
        if(!slashSFX->isPlaying()){
            delete slashSFX;
        }
    });
}

void PlayerSlash::checkCollide() {
    QList<QGraphicsItem *> collisions = scene->collidingItems(this);
    int n = collisions.length();
    for(int i = 0; i < n; i++) {
        Entity* testEntity = dynamic_cast<Entity *>(collisions[i]);
        if (testEntity) {
            if (testEntity != player && !entityDamagedList.contains(testEntity)) {
                entityDamagedList.append(testEntity);
                damage = currentAttackIndex == 2 ? 7 : 4; //if it's the third slash, it's make more damage
                testEntity->takeDamage(damage, this->player);
            }
        }
    }
}


///PAINTING METHODS

QRectF PlayerSlash::boundingRect() const
{
    QSize size = currentPixmap.size();
    return QRectF(-size.width() / 2, -size.height() / 2, size.width(), size.height());
}

QPainterPath PlayerSlash::shape() const
{
    QPainterPath path;
    QSize size = currentPixmap.size();
    switch (currentAttackIndex)
    {
        case 0:
            path.addEllipse(-size.width() / 8, -size.height() / 2, size.width() / 3, size.height());
            break;
        case 1:
            path.addEllipse(-size.width() / 8, -size.height() / 2, size.width() / 3, size.height());
            break;
        case 2:
            path.addRect(-size.width() / 2, -size.height() / 8, size.width() / 1.25, size.height() / 4);
            break;
        default:
            break;
    }
    QTransform transform;
    transform.rotate(rotationAngle);
    return transform.map(path);
}

void PlayerSlash::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    QSize size = currentPixmap.size();
    qreal width = size.width();
    qreal height = size.height();
    painter->save();
    // Appliquer la rotation directement, sans translation supplémentaire
    // car le boundingRect est maintenant centré sur (0,0)
    painter->translate(boundingRect().center());
    painter->rotate(rotationAngle);
    painter->translate(-boundingRect().center());
    painter->drawPixmap(-width / 2, -height / 2, width, height, attackAnimation[currentAttackIndex]->currentPixmap());
    painter->restore();

    // Debug - rectangle de délimitation
    painter->setPen(QPen(Qt::red, 1, Qt::DashLine));
    painter->drawRect(boundingRect());

    // Debug - shape (hitbox)
    painter->setPen(QPen(Qt::blue, 1));
    painter->drawPath(shape());
}
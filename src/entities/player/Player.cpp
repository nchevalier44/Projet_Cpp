#include "Player.h"
#include <QTimer>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QAbstractAnimation>
#include <QThread>



Player::Player(std::string name, int life, QGraphicsItem* parent) : Entity(name, life, parent) {
    this->maxHp = life;
    setAnimation(PATH_PLAYER_FRONT_IDLE, 8, 100);
    movingSound = new QSoundEffect(this);
    movingSound->setSource(QUrl::fromLocalFile(PATH_PLAYER_FOOTSTEP_SOUND));
    movingSound->setLoopCount(QSoundEffect::Infinite);
    movingSound->setVolume(30);
}


void Player::takeDamage(int damage) {
    if(isDead) return;

    //Create a red screen to indicate damage
    QWidget* DamageScreen = new QWidget(mainView);
    DamageScreen->setStyleSheet("background-color: red;");
    DamageScreen->setGeometry(mainView->rect());
    DamageScreen->show();

    //Fade of black background
    QGraphicsOpacityEffect* fadeBackgroundEffect = new QGraphicsOpacityEffect(DamageScreen);
    DamageScreen->setGraphicsEffect(fadeBackgroundEffect);
    QPropertyAnimation* animationBackground = new QPropertyAnimation(fadeBackgroundEffect, "opacity", DamageScreen);
    animationBackground->setDuration(150);
    animationBackground->setStartValue(0);
    animationBackground->setEndValue(0.5);
    animationBackground->start(QAbstractAnimation::DeleteWhenStopped);
    QTimer::singleShot(150, [DamageScreen]() {
        DamageScreen->deleteLater();
    });

    setHp(hp - damage);
    if(hud != nullptr) hud->getHPWidget()->setLife(hp);
    if(hp <= 0){
        isDead = true;
        QTimer::singleShot(1000, mainView, &MainView::displayDeathScreen);
    }
}

void Player::shootProjectile(QPointF target, QGraphicsScene* scene) {
    //Ajust the position of the projectile
    QPointF posInit = this->pos();
    posInit.setX(posInit.x() + frameWidth/20);
    posInit.setY(posInit.y() + frameHeight/20);
    QPointF direction = target - posInit;

    //Adjusting the position of the projectile to make it appear in front of the player

    switch (currentDirection) {
        case Up : posInit.setY(posInit.y() - 12); posInit.setX(posInit.x() + 7); break;
        case Down : posInit.setY(posInit.y() + 7); posInit.setX(posInit.x() + 7); break;
        case Left : posInit.setX(posInit.x() - 2); break;
        case Right : posInit.setX(posInit.x() + 15); break;
        default: break;
    }
    PlayerProjectile* projectile = new PlayerProjectile(0,3, 200, PATH_MISSILE_SPELL_GROW_ANIMATION, posInit, direction);
    projectile->setZValue(10);
    projectile->setScale(0.5);
    scene->addItem(projectile);

}

bool Player::canShoot(QPointF clickPos){
    //The player can only shoot in a 90° angle in front of him
    //We check if the player can shoot
    QPointF playerDir;
    QPointF playerPos = this->pos();
    switch(this->getCurrentDirection()) {
        case Up: playerDir = QPointF(0, -1); break;
        case Down: playerDir = QPointF(0, 1); break;
        case Left: playerDir = QPointF(-1, 0); break;
        case Right: playerDir = QPointF(1, 0); break;
        default: return false; // No direction, no shooting
    }

    QPointF dirClick = clickPos - playerPos;
    qreal lenClick = std::hypot(dirClick.x(), dirClick.y());
    if(lenClick == 0) return false; // No click position
    QPointF normClick = dirClick / lenClick;

    qreal dot = normClick.x() * playerDir.x() + normClick.y() * playerDir.y();
    qreal angleDeg = qRadiansToDegrees(qAcos(dot));

    if(angleDeg < 45){
        return true;
    }
    else{

        return false;
    }
}

void Player::slashAttack(QPointF target, QGraphicsScene* scene){
    QPointF direction = target - this->pos();
    slash->slashAttack(target, this->pos(), currentDirection);
}


///###################PROJECTILEPLAYER METHODS######################

PlayerProjectile::PlayerProjectile(int damage, int speed, int distanceMax, QString spriteSheet, QPointF pos, QPointF direction, QGraphicsObject* parent)
    :Projectile(damage, speed, distanceMax, spriteSheet, pos, direction, parent) {
    frameWidth = 32;
    frameHeight = 21;
    QPointF centerOffset(frameWidth / 2, frameHeight / 2);
    this->setPos(pos - centerOffset);
    throwProjectile();
}

void PlayerProjectile::throwProjectile() {
    setStartAnimation(PATH_PLAYER_PROJECTILE_GROW, 6, 100);
}

void PlayerProjectile::setStartAnimation(QString spriteSheet, int frameCount, int animationSpeed) {
    if(movie){
        movie->stop();
        delete movie;
        movie = nullptr;
    }
    this->movie = new QMovie(this);
    this->movie->setFileName(spriteSheet);
    this->movie->start();

    //Add a singleshot timer
    QTimer::singleShot(frameCount*animationSpeed, this, &PlayerProjectile::startMove);

}

void PlayerProjectile::setMiddleAnimation(QString spriteSheet, int frameCount, int animationSpeed) {
    if(movie){
        movie->stop();
        delete movie;
        movie = nullptr;
    }
    this->movie = new QMovie(this);
    this->movie->setFileName(spriteSheet);
    this->movie->start();
    //Add a singleshot timer
}

void PlayerProjectile::setEndAnimation(QString spriteSheet, int frameCount, int animationSpeed) {
    if(movie){
        movie->stop();
        delete movie;
        movie = nullptr;
    }
    this->movie = new QMovie(this);
    this->movie->setFileName(PATH_PLAYER_PROJECTILE_FADE);
    this->movie->start();
    //Add a singleshot timer
    frameCount = 5;
    animationSpeed = 100;
    QTimer::singleShot(frameCount*animationSpeed, this, &PlayerProjectile::deleteProjectile);
}

void PlayerProjectile::startMove() {
    setMiddleAnimation(PATH_PLAYER_PROJECTILE);
    //Starting the moving

    connect(this->timer, &QTimer::timeout, this, &Projectile::move);
    this->timer->start(30);

}

QRectF Projectile::boundingRect() const {
    return QRectF(0, 0, 40, 40);
}

QPainterPath Projectile::shape() const {
    QPainterPath path;
    // Hitbox circulaire plus petite que l'image
    path.addEllipse(boundingRect().center(), 8, 8);  // rayon 8px
    return path;
}

void Projectile::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    // Affichage de l'image actuelle de l'animation
    if (movie && !movie->currentPixmap().isNull()) {
        painter->save();
        painter->translate(boundingRect().center());
        painter->rotate(rotationAngle);
        painter->translate(-boundingRect().center());
        painter->drawPixmap(0, 10, frameWidth, frameHeight, movie->currentPixmap());
        painter->restore();
    }


    // Debug : dessiner boundingRect (en rouge)
    painter->setPen(QPen(Qt::red, 1, Qt::DashLine));
    painter->drawRect(boundingRect());

    // Debug : dessiner shape (en bleu)
    painter->setPen(QPen(Qt::blue, 1));
    painter->drawPath(shape());


}


///###################SLASHATTACK PLAYER METHODS######################

PlayerSlash::PlayerSlash(QGraphicsScene* scene) : scene(scene) {
    //We load the slashAnimation
    scene->addItem(this);
    QMovie *slash1 = new QMovie(PATH_PLAYER_SLASH1);
    QMovie *slash2 = new QMovie(PATH_PLAYER_SLASH2);
    QMovie *slash3 = new QMovie(PATH_PLAYER_SLASH3);
    slash1->start();
    slash2->start();
    slash3->start();

    /*
    slash1->setScaledSize(QSize(30, 60));
    slash2->setScaledSize(QSize(30, 60));
    slash3->setScaledSize(QSize(80, 40));
     */


    attackAnimation.push_back(slash1);
    attackAnimation.push_back(slash2);
    attackAnimation.push_back(slash3);

    for (QMovie* movie : attackAnimation) {
        connect(movie, &QMovie::frameChanged, this, [this, movie](int frameNumber) {
            if (attackAnimation[currentAttackIndex] == movie) {
                currentPixmap = movie->currentPixmap();
                update();
                if (frameNumber == movie->frameCount() - 1) {
                    this->setVisible(false);
                }
            }
        });
    }


    this->setScale(0.1);

}

void PlayerSlash::slashAttack(QPointF pos, QPointF playerPos, Direction CurrentDirection){

    //Calculate the angle to properly rotate the slash
    attackPosition = pos;
    QPointF direction = attackPosition - playerPos;
    this->rotationAngle = std::atan2(direction.y(), direction.x()) * 180 / M_PI ;

    //We check if the combo is stil valid
    if (combotimer.isValid() && combotimer.elapsed() > comboMaxDelay) {
        currentAttackIndex = 0;
    }
    // Positionnement de l'attaque
    qreal length = std::sqrt(direction.x() * direction.x() + direction.y() * direction.y());
    if (length > 0) {
        direction /= length;
    }

    //adjuste the position of the attack according to the current direction



    qreal distance = 10; // distance à laquelle l'effet est affiché
    QPointF finalPos = playerPos + direction * distance;
    switch(CurrentDirection){
        case Up : finalPos.setX(finalPos.x() + 10); finalPos.setY(finalPos.y() - 5); break;
        case Down : finalPos.setY(finalPos.y() + 25); finalPos.setX(finalPos.x() + 10); break;
        case Left : finalPos.setY(finalPos.y() + 10); break;
        case Right : finalPos.setX(finalPos.x() + 25); finalPos.setY(finalPos.y() + 10);break;
        default: break;

    }
    this->setPos(finalPos);

    this->setVisible(true);
    //Playing the attack
    playAttackAnimation(playerPos);
    currentAttackIndex = (currentAttackIndex + 1) % attackAnimation.size();
    combotimer.restart();
}

void PlayerSlash::playAttackAnimation(QPointF playerPos){

    QMovie* movie = attackAnimation[currentAttackIndex];
    movie->stop();
    movie->start();

}


QRectF PlayerSlash::boundingRect() const{
    QSize size = currentPixmap.size();
    return QRectF(-size.width()/2, -size.height()/2, size.width(), size.height());
}

QPainterPath PlayerSlash::shape() const{
    QPainterPath path;
    QSize size = currentPixmap.size();
    switch (currentAttackIndex) {
        case 0:
            path.addEllipse(-size.width()/8, -size.height()/2, size.width()/3, size.height());
            break;
        case 1:
            path.addEllipse(-size.width()/8, -size.height()/2, size.width()/3, size.height());
            break;
        case 2:
            path.addRect(-size.width()/2, -size.height()/8, size.width()/1.25, size.height()/4);
            break;
        default:
            break;
    }
    QTransform transform;
    transform.rotate(rotationAngle);
    return transform.map(path);
}

void PlayerSlash::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    QSize size = currentPixmap.size();
    qreal width = size.width();
    qreal height = size.height();
    // Définir le centre pour la rotation et le débogage
    QPointF center(width / 2.0, height / 2.0);



    painter->save();

    // Appliquer la rotation directement, sans translation supplémentaire
    // car le boundingRect est maintenant centré sur (0,0)
    painter->translate(boundingRect().center());
    painter->rotate(rotationAngle);
    painter->translate(-boundingRect().center());
    // Dessiner l'image centrée à l'origine
    painter->drawPixmap(-width/2, -height/2, width, height, attackAnimation[currentAttackIndex]->currentPixmap());
    painter->restore();

    /*
    // Debug - rectangle de délimitation
    painter->setPen(QPen(Qt::red, 1, Qt::DashLine));
    painter->drawRect(boundingRect());

    // Debug - shape (hitbox)
    painter->setPen(QPen(Qt::blue, 1));
    painter->drawPath(shape());
*/


}
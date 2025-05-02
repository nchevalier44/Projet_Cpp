#include "PlayerSlash.h"
#include "../../core/GameScene.h"

PlayerSlash::PlayerSlash(GameScene* scene) : scene(scene) {
    //We load the slashAnimation
    scene->addItem(this);
    QMovie* slash1 = new QMovie(this);
    QMovie* slash2 = new QMovie(this);
    QMovie* slash3 = new QMovie(this);
    slash1->setFileName(PATH_PLAYER_SLASH1);
    slash2->setFileName(PATH_PLAYER_SLASH2);
    slash3->setFileName(PATH_PLAYER_SLASH3);

    slash1->start();
    slash2->start();
    slash3->start();

    //slash1->setScaledSize(QSize(30, 60));
    //slash2->setScaledSize(QSize(30, 60));
    //slash3->setScaledSize(QSize(80, 40));



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
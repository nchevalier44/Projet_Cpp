#include "Player.h"
#include "PlayerShield.h"
#include "../../core/GameScene.h"



PlayerShield::PlayerShield(GameScene* scene,Player* player, QGraphicsObject* parent) : scene(scene), player(player), QGraphicsObject(parent){
    scene->removeItem(this);
    scene->addItem(this);

    this->setZValue(50);

    movie = new QMovie(PATH_PLAYER_SHIELD);

    movie->start();
    movie->jumpToFrame(0);

    //Setting up the size
    QSize originalSize = movie->currentPixmap().size();
    QSizeF targetSize = player->shape().boundingRect().size();

    float scaleWidth = targetSize.width() / originalSize.width();
    float scaleHeight = targetSize.height() / originalSize.height();
    float scaleFactor = std::min(scaleWidth, scaleHeight);

    QSize scaledSize = originalSize * scaleFactor * 0.55;

    // 3. Applique la nouvelle taille
    movie->setScaledSize(scaledSize);

    connect(movie, &QMovie::frameChanged, this, [this](int){
        currentPixmap = movie->currentPixmap();
        update(); // Redessine le bouclier à chaque frame
    });

    movie->start();

    timerShield = new QTimer(this);
    connect(timerShield, &QTimer::timeout, this, &PlayerShield::updatePosition);
    this->hide();
}

void PlayerShield::updatePosition() {
    if (player) {
        // Center the shield on the player
        QRectF shape = player->shape().boundingRect();
        this->setPos(player->pos().x() - shape.width() / 14, player->pos().y() - shape.height() / 20);
    }
}

void PlayerShield::activeShield() {
    if(isActivated){
        return;
    }
    isActivated = true;
    timerShield->start(16);
    this->show();
    QTimer::singleShot(durability, this, &PlayerShield::desactiveShield);
}

void PlayerShield::desactiveShield() {
    isActivated = false;
    timerShield->stop();
    this->hide();
    HP = maxHP;
}


///PAINTING METHODS

QRectF PlayerShield::boundingRect() const {
    QSize size = currentPixmap.size();
    return QRectF(0, 0, size.width(), size.height());
}

QPainterPath PlayerShield::shape() const {
    if(!isActivated){
        return QPainterPath();
    }

    QSize size = currentPixmap.size();
    QPainterPath path;
    path.addEllipse(0,0,size.width(),size.height());
    return path;
}

void PlayerShield::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    // Draw the current frame of the sprite sheet
    painter->drawPixmap(0, 0, currentPixmap);
}


#include "PlayerShield.h"


Shield::Shield() {
    movie = new QMovie(PATH_PLAYER_SHIELD);
    }




///PAINTING METHODS

QRectF Shield::boundingRect() const {
    QSize size = currentPixmap.size();
    return QRectF(0, 0, size.width(), size.height());
}
QPainterPath Shield::shape() const {
    QSize size = currentPixmap.size();
    QPainterPath path;
    path.addEllipse(0,0,size.width(),size.height());
    return path;
}

void Shield::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    // Draw the current frame of the sprite sheet
    QRect sourceRect(0,0,currentPixmap.width(), currentPixmap.height());
    painter->drawPixmap(0, 0, currentPixmap.width(), currentPixmap.height(), currentPixmap, sourceRect.x(), sourceRect.y(), sourceRect.width(), sourceRect.height());
    painter->drawRect(boundingRect()); // Optional: Draw the bounding rect for debugging

    painter->setPen(QPen(Qt::green, 2, Qt::DashLine));
    painter->drawPath(shape());

}

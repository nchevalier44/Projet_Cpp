#include "MainMenuButton.h"
#include <QMovie>
#include <QEnterEvent>
#include <QPainter>
#include <QStyleOptionButton>

MainMenuButton::MainMenuButton(QString text, QWidget* parent): QPushButton(text, parent) {
    setStyleSheet("color: white;");
}

MainMenuButton::~MainMenuButton() {
    delete hoverMovie;
}

void MainMenuButton::setGifPath(const QString& path){
    if (hoverMovie) delete hoverMovie;
    hoverMovie = new QMovie(path);
    hoverMovie->setCacheMode(QMovie::CacheAll);
    hoverMovie->setSpeed(200);

    connect(hoverMovie, &QMovie::frameChanged, this, [this]() {
        if(hoverMovie->currentFrameNumber() == hoverMovie->frameCount() - 1){
            hoverMovie->stop();
        }

        update();  // Redessine le bouton avec la dernière frame
    });
}

void MainMenuButton::enterEvent(QEnterEvent* event) {
    QPushButton::enterEvent(event);
    hover = true;
    if (hoverMovie) hoverMovie->start();

    update();
}

void MainMenuButton::leaveEvent(QEvent* event) {
    QPushButton::leaveEvent(event);
    hover = false;
    if (hoverMovie) hoverMovie->stop();
    update(); // Pour forcer le repaint sans le GIF
}

void MainMenuButton::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    QPushButton::paintEvent(event);

    if (hover && hoverMovie && hoverMovie->isValid()) {
        QPixmap currentFrame = hoverMovie->currentPixmap();
        //painter.drawPixmap(rect(), currentFrame);
        painter.drawPixmap(rect(), currentFrame.scaled(this->size(), Qt::KeepAspectRatio));
    } else if(!background.isNull()){
         painter.drawPixmap(rect(), background);
     }


    QStyleOptionButton option;
    option.initFrom(this);
    option.text = this->text();
    option.icon = this->icon();
    style()->drawControl(QStyle::CE_PushButtonLabel, &option, &painter, this);

}

void MainMenuButton::resizeEvent(QResizeEvent* event) {
    QSize newSize = event->size();
    this->setFixedSize(newSize.width() * 2.5, newSize.height() * 2.5);
}
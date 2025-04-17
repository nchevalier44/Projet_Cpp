#include "MainMenuButton.h"
#include <QMovie>
#include <QEnterEvent>
#include <QPainter>
#include <QStyleOptionButton>

MainMenuButton::MainMenuButton(QString text, QWidget* parent): QPushButton(text, parent) {
    //Set color of the text white
    setStyleSheet("color: white;");

    //Setup the background image and hover animation
    backgroundPixmap = new QPixmap("../assets/images/menu/button_background.png");
    hoverMovie = new QMovie("../assets/images/menu/button_animation.gif");

    connect(hoverMovie, &QMovie::frameChanged, this, [this]() {
        //We stop at the last frame in order to get only one loop
        if(hoverMovie->currentFrameNumber() == hoverMovie->frameCount() - 1){
            hoverMovie->stop();
        }
        //
        update(); //We update to let the last frame as the background while hovering
    });
}

MainMenuButton::~MainMenuButton() {
    delete hoverMovie;
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
    update();
}

void MainMenuButton::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    QPushButton::paintEvent(event);

    if (hover && hoverMovie && hoverMovie->isValid()) {
        QPixmap currentFrame = hoverMovie->currentPixmap();
        painter.drawPixmap(rect(), currentFrame.scaled(rect().size()));
    } else if(!backgroundPixmap->isNull()){
         painter.drawPixmap(rect(), backgroundPixmap->scaled(rect().size()));
     }

    QStyleOptionButton option;
    option.initFrom(this);
    option.text = this->text();
    option.icon = this->icon();
    style()->drawControl(QStyle::CE_PushButtonLabel, &option, &painter, this);

}

void MainMenuButton::resizeEvent(QResizeEvent* event) {
    QPushButton::resizeEvent(event);
    QSize newSize = event->size();
    this->setFixedSize(newSize.width() * 2.5, newSize.height() * 2);
}
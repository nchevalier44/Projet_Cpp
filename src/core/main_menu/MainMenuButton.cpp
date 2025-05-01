#include "MainMenuButton.h"

//Constructor
MainMenuButton::MainMenuButton(QString text, QWidget* parent): QPushButton(text, parent) {
    //Set color of the text white
    setStyleSheet("color: white;");

    //Setup the background image and hover animation
    backgroundPixmap = new QPixmap(PATH_MAIN_MENU_BUTTON_BACKGROUND);
    hoverMovie = new QMovie(this);
    hoverMovie->setFileName(PATH_MAIN_MENU_BUTTON_ANIMATION);

    //We check each frame of the animation to see if it's the last one
    connect(hoverMovie, &QMovie::frameChanged, this, [this]() {
        //We stop at the last frame in order to get only one loop
        if(hoverMovie->currentFrameNumber() == hoverMovie->frameCount() - 1){
            hoverMovie->stop();
        }
        update(); //We update to let the last frame as the background while hovering
    });
}

//Destructor
MainMenuButton::~MainMenuButton() {
    delete backgroundPixmap;
    backgroundPixmap = nullptr;
}

//Redefinition of the enterEvent function to start the hover animation
void MainMenuButton::enterEvent(QEnterEvent* event) {
    QPushButton::enterEvent(event); //We call de default enterEvent function
    isHovering = true;
    if(hoverMovie && hoverMovie->isValid()) {
        hoverMovie->start();
    } else{
        qWarning() << "The hover animation is not valid";
    }
    update(); //We force the update to see the animation
}

//Redefinition of the leaveEvent function to stop the hover animation
void MainMenuButton::leaveEvent(QEvent* event) {
    QPushButton::leaveEvent(event); //We call de default leaveEvent function
    isHovering = false;
    if(hoverMovie && hoverMovie->isValid()){
        hoverMovie->stop();
    } else{
        qWarning() << "The hover animation is not valid";
    }
    update();//We force the update to stop the animation or return to the background image
}

//Redefinition of the paintEvent function to draw the background image and the hover animation
void MainMenuButton::paintEvent(QPaintEvent* event) {
    QPainter painter(this);

    if (isHovering && hoverMovie && hoverMovie->isValid()) { //Draw the hover animation at the good frame and the good size
        QPixmap currentFrame = hoverMovie->currentPixmap();
        painter.drawPixmap(rect(), currentFrame.scaled(rect().size()));
    } else if(backgroundPixmap && !backgroundPixmap->isNull()){
         painter.drawPixmap(rect(), backgroundPixmap->scaled(rect().size())); //Draw the background image at the good size
     } else{
        QPushButton::paintEvent(event);//We call the default paintEvent function
        qWarning() << "The hover animation or background image is not valid ";
    }

    QStyleOptionButton option;
    option.initFrom(this);
    option.text = this->text();
    style()->drawControl(QStyle::CE_PushButtonLabel, &option, &painter, this); //Draw just the text
}

//Redefinition of the resizeEvent function to resize the button with different scale
void MainMenuButton::resizeEvent(QResizeEvent* event) {
    QPushButton::resizeEvent(event);
    QSize newSize = event->size();
    this->setFixedSize(newSize.width() * 2.5, newSize.height() * 2);
}
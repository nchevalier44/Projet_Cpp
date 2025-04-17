#include "VolumeSlider.h"

//Constructor
VolumeSlider::VolumeSlider(Qt::Orientation orientation, QWidget* parent) : QSlider(orientation, parent){
    //Set minimum and maximum value
    this->setMinimum(0);
    this->setMaximum(100);

    //Set the images and their ratio
    barPixmap = new QPixmap(PATH_MAIN_MENU_SLIDER_BAR);
    handlePixmap = new QPixmap(PATH_MAIN_MENU_SLIDER_HANDLE);
    ratioHandle = float(handlePixmap->width()) / float(handlePixmap->height());
    ratioBar = float(barPixmap->width()) / float(barPixmap->height());
}

//Destructor
VolumeSlider::~VolumeSlider() {
    delete barPixmap;
    barPixmap = nullptr;
    delete handlePixmap;
    handlePixmap = nullptr;
}

//Redifinition of the paintEvent function to draw the bar and hande with the right ratio
void VolumeSlider::paintEvent(QPaintEvent* event) {
    QSlider::paintEvent(event);
    QPainter painter(this);

    //Draw the bar of the slider (all the width)
    if(barPixmap && !barPixmap->isNull()){
        painter.drawPixmap(0, 0, barPixmap->scaled( QSize(width(), width() / ratioBar)));
    } else{
        qWarning() << "The bar image is not valid";
    }

    //Draw the slider handle
    if(handlePixmap && !handlePixmap->isNull()){
        float newHandleWidth = ratioHandle * height();
        float ratioPosition = float(value() - minimum()) / float(maximum() - minimum());
        int handlePosition = ratioPosition * (width() - newHandleWidth);
        painter.drawPixmap(handlePosition, 0, handlePixmap->scaled( QSize(newHandleWidth, height())));
    } else{
        qWarning() << "The handle image is not valid";
    }
}

//Redifinition of the resizeEvent function to resize the slider with the right ratio
void VolumeSlider::resizeEvent(QResizeEvent* event) {
    QSlider::resizeEvent(event);
    int newHeight = float(event->size().width()) / float(ratioBar);
    if (height() != newHeight) {
        setFixedHeight(newHeight);
    }
}
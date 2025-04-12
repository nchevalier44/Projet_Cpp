#include "HUD.h"



HUD::HUD(int maxHP, QWidget* parent) : QWidget(parent) {
        //Base attribute of HUD
        setAttribute(Qt::WA_OpaquePaintEvent);
        setAttribute(Qt::WA_TranslucentBackground);
        setAttribute(Qt::WA_TransparentForMouseEvents);
        setAttribute(Qt::WA_NoSystemBackground);
        setStyleSheet("background: transparent;");

        //Setting up the HUD using a layout
        QHBoxLayout* layout = new QHBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);


        //HPWidget
        hpWidget = new HPWidget(maxHP, this);
        //hpWidget->setAttribute(Qt::WA_OpaquePaintEvent);
        hpWidget->move(10, 10);

        //Adding every HUD element to the layout
        layout->addWidget(hpWidget);

        //Adding the layout to the HUD
        this->setLayout(layout);

        show();
}



///HEART HUD

HPWidget::HPWidget(int maxLife, QWidget *parent) : QWidget(parent), maxLife(maxLife) {
    QHBoxLayout* lifeLayout = new QHBoxLayout(this);
    lifeLayout->setContentsMargins(0, 0, 0, 0);



    for(int i = 0 ; i < maxLife ; i++){
        if(i < maxLife){
            QMovie* heart = new QMovie("../assets/images/characters/Fire_head.gif");
            QLabel* label = new QLabel();

            label->setMovie(heart);
            heart->start();
            label->setAttribute(Qt::WA_OpaquePaintEvent);
            life.append(label);
            lifeLayout->addWidget(label);
        }

    }
    setLife(maxLife-2);
    this->setLayout(lifeLayout);

}

void HPWidget::setLife(int hp) {
    QMovie* lifeMovie = new QMovie("../assets/images/characters/Fire_head.gif");
    QMovie* emptyLifeMovie = new QMovie("../assets/images/characters/Fireless_head.gif");
    for(int i = 0 ; i < maxLife ; i++){
        if(i < hp){
            life[i]->setAttribute(Qt::WA_OpaquePaintEvent);
            life[i]->setMovie(lifeMovie);
            lifeMovie->start();
        }
        else{
            life[i]->setAttribute(Qt::WA_OpaquePaintEvent);
            life[i]->setMovie(emptyLifeMovie);
            emptyLifeMovie->start();
        }
    }
}
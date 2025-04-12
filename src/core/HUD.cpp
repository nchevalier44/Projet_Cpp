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
    lifeLayout->setContentsMargins(10, 10, 10, 10);
    lifeLayout->setSpacing(5);


    for(int i = 0 ; i < maxLife ; i++){
        if(i < maxLife){
            QMovie* heart = new QMovie("../assets/images/characters/Fire_head.gif");
            QLabel* label = new QLabel();

            label->setMovie(heart);
            heart->start();
            label->setAttribute(Qt::WA_OpaquePaintEvent);

            lifeLayout->addWidget(label);
        }

    }
    this->setLayout(lifeLayout);

}

void HPWidget::setLife(int hp) {
    for(int i = 0 ; i < maxLife ; i++){
        if(i < hp){
            life[i]->setFileName("../assets/images/characters/Fire_head.gif");
        }
        else{
            life[i]->setFileName("../assets/images/characters/Dead_head.png");
        }
    }
}
#include <QPropertyAnimation>
#include <QFontDatabase>
#include "HUD.h"



HUD::HUD(int maxHP, QPointF windowSize, QWidget* parent): QWidget(parent) {
    //Base attribute of HUD
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setAttribute(Qt::WA_NoSystemBackground);
    setStyleSheet("background: transparent;");

    //HP Widget
    hpWidget = new HPWidget(maxHP,windowSize, this);
    hpWidget->move(windowSize.x() * 0.01, windowSize.y() * 0.01);

    //Spell Widget
    spellWidget = new SpellWidget(1,windowSize, this);
    spellWidget->move(windowSize.x() - windowSize.x() * 0.2 - windowSize.x() * 0.02, windowSize.y() * 0.02);

    show();
}



///HEART HUD

HPWidget::HPWidget(int maxLife, QPointF windowSize, QWidget *parent) : QWidget(parent), maxLife(maxLife) {
    QHBoxLayout* lifeLayout = new QHBoxLayout(this);
    const int heartWidth = windowSize.x() * 0.05;   //Change this to adjust the size of the heart
    const int heartHeight = windowSize.y() * 0.12; //Always twice the width
    const int spacing = windowSize.x() * 0.01;

    //Keep the ration according to the size of the window
    this->setFixedSize(maxLife * (heartWidth + spacing), heartHeight);
    for(int i = 0 ; i < maxLife ; i++){
        if(i < maxLife){
            QMovie* heart = new QMovie("../assets/images/characters/Fire_head.gif");
            QLabel* label = new QLabel();

            label->setFixedSize(heartWidth, heartHeight);
            heart->setScaledSize(label->size());
            label->setMovie(heart);
            heart->start();
            label->setScaledContents(true);
            life.append(label);
            lifeLayout->addWidget(label,0, Qt::AlignCenter);
        }
    }
    setLife(maxLife);
    this->setLayout(lifeLayout);
}

void HPWidget::setLife(int hp) {
    QMovie* lifeMovie = new QMovie("../assets/images/characters/Fire_head.gif");
    QMovie* emptyLifeMovie = new QMovie("../assets/images/characters/Fireless_head.gif");
    for(int i = maxLife-1 ; i >= 0 ; i--){
        if(i < hp){
            life[i]->setMovie(lifeMovie);
            lifeMovie->start();
        }
        else{
            life[i]->setMovie(emptyLifeMovie);
            emptyLifeMovie->start();
        }
    }
}


///SPELL HUD

SpellWidget::SpellWidget(int maxSpell, QPointF windowSize, QWidget *parent) : QWidget(parent), maxSpell(maxSpell){
    //Base attribute of the widget
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_TransparentForMouseEvents);

    selectedSpell.resize(maxSpell, false);

    QVBoxLayout* spellLayout = new QVBoxLayout(this);
    spellLayout->setAlignment(Qt::AlignRight | Qt::AlignTop);

    //set the size of the widget according to the size of the window
    this->setFixedSize(windowSize.x() * 0.2, windowSize.y() * 0.2 * maxSpell + 50);
    for (int i = 0; i < maxSpell; ++i) {
        // Créer une boîte pour chaque sort
        QLabel* spellLabel = new QLabel();
        QPixmap spellPixmap;
        if(i == 0){
            selectedSpell[0] = true;
            spellPixmap = QPixmap(PATH_SPELL_BOX_SELECTED);
        }
        else{
            selectedSpell[i] = false;
            spellPixmap = QPixmap(PATH_SPELL_BOX);
        }

        spellLabel->setFixedSize(windowSize.x() * 0.1, windowSize.y() * 0.1);
        spellLabel->setPixmap(spellPixmap.scaled(spellLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

        // Ajouter une icône de sort
        QLabel* iconeLabel = new QLabel(spellLabel);
        QPixmap iconePixmap = QPixmap(PATH_MISSILE_SPELL_ICONE);
        iconeLabel->setFixedSize(spellLabel->width() * 0.8, spellLabel->height() * 0.8);
        iconeLabel->setPixmap(iconePixmap.scaled(iconeLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        iconeLabel->move(spellLabel->width() / 2 - iconeLabel->width() / 2, spellLabel->height() / 2 - iconeLabel->height() / 2);

        //Ajout du nombre de missile restant
        missileCountLabel = new QLabel(spellLabel);
        int fontId = QFontDatabase::addApplicationFont(PATH_JERSEY10_FONT);
        QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
        QFont font(fontFamily);
        font.setPointSize(24); // Taille de la police (ajuste selon ton besoin)
        missileCountLabel->setFont(font);
        missileCountLabel->setStyleSheet("color: white; font-size: 20px;");
        missileCountLabel->move(spellLabel->width() - 40, spellLabel->height() - 25);
        missileCountLabel->setText(QString::number(currentMissile));
        missileCountLabel->raise();
        missileCountLabel->show();
        // Ajouter le widget au layout
        spellLayout->addWidget(spellLabel);
        spell.append(spellLabel);

    }
    this->setLayout(spellLayout);
}

void SpellWidget::shootedMissile(){
    QLabel* spellLabel = spell[0];
    const int spellLabelWidth = spellLabel->pixmap().width();
    const int spellLabelHeight = spellLabel->pixmap().height();
    currentMissile--;
    missileCountLabel->setText(QString::number(currentMissile));


    QLabel* cooldownOverlay = new QLabel(spellLabel);
    cooldownOverlay->setStyleSheet("background-color: rgba(0, 0, 0, 150);");
    cooldownOverlay->setGeometry(0, 0,spellLabelWidth, spellLabelHeight); // Geometry dès le début
    cooldownOverlay->show();
    cooldownOverlay->raise();

    QPropertyAnimation* animation = new QPropertyAnimation(cooldownOverlay, "geometry");
    animation->setDuration(2000);
    animation->setStartValue(QRect(0, 0,spellLabelWidth, spellLabelHeight));
    animation->setEndValue(QRect(0, spellLabelHeight, spellLabelWidth, 0));
    animation->setEasingCurve(QEasingCurve::OutQuad);

    //animation->setEasingCurve(QEasingCurve::OutQuad);
    animation->start(QAbstractAnimation::DeleteWhenStopped);

    QTimer::singleShot(2000, [cooldownOverlay, this]() {
        cooldownOverlay->deleteLater();
        setCurrentMissile(currentMissile + 1);
        missileCountLabel->setText(QString::number(currentMissile));
    });
}

void SpellWidget::changeSelectedSpell(int spellIndex){
    QPixmap selectedSpellPixmap;

    for(int i = 0 ; i < maxSpell ; i++){
        if(i == spellIndex) {
            if(selectedSpell[i]) {
                return; // Already selected
            }
            else {
                selectedSpellPixmap = QPixmap(PATH_SPELL_BOX_SELECTED);
                spell[spellIndex]->setPixmap(selectedSpellPixmap.scaled(spell[i]->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
                selectedSpell[i] = true;
            }
        }
        else if(selectedSpell[i]) {
            selectedSpellPixmap = QPixmap(PATH_SPELL_BOX);
            selectedSpell[i] = false;
            spell[i]->setPixmap(selectedSpellPixmap.scaled(spell[i]->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

        }
    }
}


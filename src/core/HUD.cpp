#include <QPropertyAnimation>
#include <QFontDatabase>
#include "HUD.h"
#include "MainWindow.h"
#include "FontManager.h"


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
    spellWidget = new SpellWidget(3,windowSize, this);
    spellWidget->move(windowSize.x() * 0.78, windowSize.y() * 0.02);

    show();
}

void HUD::updateHUD(int actualHP, int newMaxHP, QPointF newWindowSize){
    this->setGeometry(0, 0, newWindowSize.x(), newWindowSize.y());
    delete hpWidget;
    hpWidget = new HPWidget(newMaxHP, newWindowSize, this);
    hpWidget->move(newWindowSize.x() * 0.01, newWindowSize.y() * 0.01);
    hpWidget->setLife(actualHP);
    hpWidget->show();
    delete spellWidget;
    spellWidget = new SpellWidget(3, newWindowSize, this);
    spellWidget->move(newWindowSize.x() * 0.78, newWindowSize.y() * 0.02);
    spellWidget->show();
    spellWidget->getSpell()[0]->show();
    spellWidget->getSpell()[1]->show();
    spellWidget->getSpell()[2]->show();
}



///HEART HUD

HPWidget::HPWidget(int maxLife, QPointF windowSize, QWidget *parent) : QWidget(parent), maxLife(maxLife) {
    lifeLayout = new QHBoxLayout(this);
    heartWidth = windowSize.x() * 0.05;   //Change this to adjust the size of the heart
    heartHeight = windowSize.y() * 0.12; //Always twice the width
    const int spacing = windowSize.x() * 0.01;

    //Keep the ration according to the size of the window
    this->setFixedSize(maxLife * (heartWidth + spacing), heartHeight);
    for(int i = 0 ; i < maxLife ; i++){
        QMovie* heart = new QMovie(this);
        heart->setFileName(PATH_HP_ANIMATION);
        QLabel* label = new QLabel(this);

        label->setFixedSize(heartWidth, heartHeight);
        heart->setScaledSize(label->size());
        label->setMovie(heart);
        heart->start();
        label->setScaledContents(true);
        life.append(label);
        lifeLayout->addWidget(label,0, Qt::AlignCenter);
        if(i > maxLife){
            label->hide();
        }
    }
    this->setLayout(lifeLayout);
}

void HPWidget::setLife(int hp) {
    for(int i = 0 ; i < maxLife ; i++) {
        QMovie* movie;
        if(i < hp){
            movie = new QMovie(PATH_HP_ANIMATION, QByteArray(), this);
        } else {
            movie = new QMovie(PATH_HP_DEAD_ANIMATION, QByteArray(), this);
        }
        movie->setScaledSize(life[i]->size());
        life[i]->setMovie(movie);
        movie->start();
    }
    qDebug() << "Life : " << hp;
}

void HPWidget::addHP() {
    ++maxLife;

    QLabel* label = new QLabel(this);
    label->setFixedSize(heartWidth, heartHeight);

    QMovie* emptyLife = new QMovie(PATH_HP_DEAD_ANIMATION, QByteArray(), this);
    emptyLife->setScaledSize(label->size());
    label->setMovie(emptyLife);
    label->setScaledContents(true);
    emptyLife->start();
    life.append(label);

    if (lifeLayout) {
        lifeLayout->addWidget(label, 0, Qt::AlignCenter);
    }
    // Redimensionner le widget principal
    this->setFixedWidth(maxLife * (heartWidth + (heartWidth * 0.2)));
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
        QLabel* spellLabel = new QLabel(this);
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
        QPixmap iconePixmap;
        if(i == 0){
            iconePixmap = QPixmap(PATH_MISSILE_SPELL_ICONE);
        }
        else if(i == 1){
            iconePixmap = QPixmap(PATH_MISSILE_SLASH_ICONE);

        }
        else if(i == 2){
            iconePixmap = QPixmap(PATH_MISSILE_SHIELD_ICONE);
        }


        iconeLabel->setFixedSize(spellLabel->width() * 0.8, spellLabel->height() * 0.8);
        iconeLabel->setPixmap(iconePixmap.scaled(iconeLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        iconeLabel->move(spellLabel->width() / 2 - iconeLabel->width() / 2, spellLabel->height() / 2 - iconeLabel->height() / 2);

        if(i == 0){
            //Ajout du nombre de missile restant
            missileCountLabel = new QLabel(spellLabel);
            QFont font(FontManager::fontFamily);
            font.setPointSize(0.035 * this->height()); // Taille de la police (ajuste selon ton besoin)
            missileCountLabel->setFont(font);
            missileCountLabel->setStyleSheet("color: white;");
            missileCountLabel->move(spellLabel->width() * 0.59, spellLabel->height() * 0.63);
            missileCountLabel->setText(QString::number(currentMissile));
            missileCountLabel->raise();
            missileCountLabel->show();
        }

        // Ajouter le widget au layout
        spellLayout->addWidget(spellLabel);
        spellLabel->hide();
        spell.append(spellLabel);
    }
    this->setLayout(spellLayout);
}

void SpellWidget::shootedMissile(){
    QLabel* cooldownOverlay = coolDownAnimation(0,2000);
    currentMissile--;
    missileCountLabel->setText(QString::number(currentMissile));
    QTimer::singleShot(2000, [cooldownOverlay, this]() {
        delete cooldownOverlay;
        setCurrentMissile(currentMissile + 1);
        missileCountLabel->setText(QString::number(currentMissile));
    });
}

void SpellWidget::shieldUsed(){
    QLabel* cooldownOverlay = coolDownAnimation(2,5000);
    isShieldOnCD = true;
    QTimer::singleShot(5000, [cooldownOverlay, this]() {
        delete cooldownOverlay;
        isShieldOnCD = false;
    });
}

QLabel* SpellWidget::coolDownAnimation(int spellSelected, double duration){
    QLabel* spellLabel = spell[spellSelected];
    const int spellLabelWidth = spellLabel->pixmap().width();
    const int spellLabelHeight = spellLabel->pixmap().height();
    QLabel* cooldownOverlay = new QLabel(spellLabel);
    cooldownOverlay->setStyleSheet("background-color: rgba(0, 0, 0, 150);");
    cooldownOverlay->setGeometry(0, 0,spellLabelWidth, spellLabelHeight); // Geometry dès le début
    cooldownOverlay->show();
    cooldownOverlay->raise();


    QPropertyAnimation* animation = new QPropertyAnimation(cooldownOverlay, "geometry", spellLabel);
    animation->setDuration(duration);
    animation->setStartValue(QRect(0, 0,spellLabelWidth, spellLabelHeight));
    animation->setEndValue(QRect(0, spellLabelHeight, spellLabelWidth, 0));
    animation->setEasingCurve(QEasingCurve::OutQuad);

    animation->start(QAbstractAnimation::DeleteWhenStopped);

    return cooldownOverlay;
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
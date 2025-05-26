#ifndef PROJET_CPP_HUD_H
#define PROJET_CPP_HUD_H

#include <QWidget>
#include <QLabel>
#include <QMovie>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLayout>
#include <QTimer>
#include "../constants.h"


class HPWidget : public QWidget {
Q_OBJECT
private:
    QVector<QLabel*> life;
    int maxLife;
    QHBoxLayout* lifeLayout;
    int heartWidth;
    int heartHeight;

public :
    //Constructor
    HPWidget(int maxLife, QPointF windowSize, QWidget* parent = nullptr);

    //Getter
    QVector<QLabel*>& getLifeList(){ return life; }
    int getMaxLife() const { return maxLife; }

    //Setter
    void setLife(int life);

    void addHP();


};

class SpellWidget : public QWidget {
Q_OBJECT
private :
    QVector<QLabel*> spell;
    QVector<bool> selectedSpell;
    QLabel* missileCountLabel;
    int maxSpell;
    int maxMissile = 3;
    int currentMissile = maxMissile;
    bool isShieldOnCD = false;

public :
    //Constructor
    SpellWidget(int maxSpell,QPointF windowSize, QWidget* parent = nullptr);

    //Getter
    int getCurrentMissile() const { return currentMissile; }
    bool getIsShieldOnCd(){return isShieldOnCD;}
    QVector<QLabel*> getSpell() const { return spell; }
    QVector<bool> getSelectedSpell() const { return selectedSpell; }

    //Setter
    void setCurrentMissile(int newCurrentMissile){currentMissile = newCurrentMissile;};

    void shootedMissile();
    void shieldUsed();
    QLabel* coolDownAnimation(int spellSelected, double duration);
    void changeSelectedSpell(int spellIndex);
};

class HUD : public QWidget {
Q_OBJECT

public :
    //Constructor
    HUD(int maxHP, QPointF windowSize, QWidget* parent = nullptr);

    //Getters
    HPWidget* getHPWidget() const { return hpWidget; };
    SpellWidget* getSpellWidget() const { return spellWidget; };

    void updateHUD(int actualHP, int newMaxHP, QPointF newWindowSize);
private:
    HPWidget* hpWidget;
    SpellWidget* spellWidget;
};

#endif //PROJET_CPP_HUD_H
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

class MainWindow;

class HPWidget : public QWidget {
Q_OBJECT
private:
    QVector<QLabel*> life;
    int maxLife;
    MainWindow* mainWindow;

public :
    HPWidget(int maxLife, MainWindow* mainWindow, QWidget* parent = nullptr);
    void setLife(int life);
    QVector<QLabel*>& getLifeList(){ return life; }

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
    MainWindow* mainWindow;

public :
    SpellWidget(int maxSpell,MainWindow* mainWindow, QWidget* parent = nullptr);
    void setCurrentMissile(int newCurrentMissile){currentMissile = newCurrentMissile;};
    int getCurrentMissile() const { return currentMissile; }
    void shootedMissile();
    void changeSelectedSpell(int spellIndex);
    QVector<bool> getSelectedSpell() const { return selectedSpell; }

};

class HUD : public QWidget {
    Q_OBJECT

public :
    HUD(int maxHP, MainWindow* mainWindow, QWidget* parent = nullptr);
    HPWidget* getHPWidget() const { return hpWidget; };
    SpellWidget* getSpellWidget() const { return spellWidget; };

    void updateItemsSize();

private:
    HPWidget* hpWidget;
    SpellWidget* spellWidget;
    MainWindow* mainWindow;
};

#endif //PROJET_CPP_HUD_H

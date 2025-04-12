#ifndef PROJET_CPP_HUD_H
#define PROJET_CPP_HUD_H

#include <QWidget>
#include <QLabel>
#include <QMovie>
#include <QHBoxLayout>


class HPWidget : public QWidget {
Q_OBJECT
private:
    QVector<QLabel*> life;
    int maxLife;

public :
    HPWidget(int maxLife, QWidget* parent = nullptr);
    void setLife(int life);

};

class HUD : public QWidget {
    Q_OBJECT

public :
    HUD(int maxHP, QWidget* parent = nullptr);
    HPWidget* getHPWidget() const { return hpWidget; };
private:
    HPWidget* hpWidget;
};

#endif //PROJET_CPP_HUD_H

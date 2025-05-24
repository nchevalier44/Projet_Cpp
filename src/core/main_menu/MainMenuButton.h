#ifndef PROJET_CPP_MAINMENUBUTTON_H
#define PROJET_CPP_MAINMENUBUTTON_H

#include <QPushButton>
#include <QMovie>
#include <QEnterEvent>
#include <QPainter>
#include <QStyleOptionButton>

#include "../../constants.h"
#include "../AudioManager.h"

class MainMenuButton : public QPushButton{
    Q_OBJECT
public:
    //Constructor and destructor
    MainMenuButton(QString text, AudioManager* audioManager, QWidget* parent=nullptr);
    virtual ~MainMenuButton();

protected:
    //Function redefined
    void paintEvent(QPaintEvent* event) override;
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;

private:
    QMovie* hoverMovie = nullptr;
    bool isHovering = false;
    QPixmap* backgroundPixmap = nullptr;
    AudioManager* audioManager = nullptr;
};


#endif //PROJET_CPP_MAINMENUBUTTON_H

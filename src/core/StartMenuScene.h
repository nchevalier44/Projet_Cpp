#ifndef PROJET_CPP_STARTMENUSCENE_H
#define PROJET_CPP_STARTMENUSCENE_H

#include <QGraphicsScene>
#include <QPushButton>
#include <QWidget>
#include <QVBoxLayout>
#include <QPixmap>
#include <QLabel>
#include <QPainter>
#include <QApplication>
#include <QFontDatabase>
#include <QGraphicsProxyWidget>
#include <iostream>


class StartMenuScene : public QGraphicsScene{
    Q_OBJECT

    public:
        void drawBackground(QPainter* painter, const QRectF& rect);
        StartMenuScene(QObject* parent = nullptr);
        virtual ~StartMenuScene() {};

    private:
        QPixmap background;
        QWidget* buttonsContainer;

    signals:
        void startGameRequested();

};


#endif //PROJET_CPP_STARTMENUSCENE_H

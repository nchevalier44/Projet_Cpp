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
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QSoundEffect>

class MenuButton : public QPushButton{
    Q_OBJECT
public:
    MenuButton(QString text, QWidget* parent=nullptr)  : QPushButton(text, parent) {};
    virtual ~MenuButton() {};

    void setImagePath(QString path) { background = QPixmap(path); };
    void paintEvent(QPaintEvent* event);

private:
    QPixmap background;
};

class StartMenuScene : public QGraphicsScene{
    Q_OBJECT

    public:
        //Getters
        QPixmap* getBackground() const { return background; }
        QMediaPlayer* getAudioPlayer() const { return audioPlayer; }

        void drawBackground(QPainter* painter, const QRectF& rect);

        StartMenuScene(QObject* parent = nullptr);
        virtual ~StartMenuScene() {};

    private:
        QPixmap* background = nullptr;
        QWidget* buttonsContainer = nullptr;
        QMediaPlayer* audioPlayer = nullptr;
        QSoundEffect* sound = nullptr;

    signals:
        void startGameRequested();

};


#endif //PROJET_CPP_STARTMENUSCENE_H

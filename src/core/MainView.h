#ifndef PROJET_CPP_MAINVIEW_H
#define PROJET_CPP_MAINVIEW_H

#include <QGraphicsView>
#include <QWheelEvent>
#include "ScoreManager.h"

class MainView : public QGraphicsView {
    Q_OBJECT
    public:
        MainView(ScoreManager* scoreManager, QWidget* parent = nullptr);
        virtual ~MainView();

        //Setters
        void setFitView(bool newFitView) { fitView = newFitView; }

        void deleteDeathScreen();

    protected:
        void resizeEvent (QResizeEvent* event) override;
        void wheelEvent(QWheelEvent* event) override { event->ignore(); }

        private:
        bool fitView = true;
        QWidget* deathScreen = nullptr;
        ScoreManager* scoreManager = nullptr;

    public slots:
        void displayDeathScreen();


signals:
        void startGameRequested();
        void goToStartMenuRequested();
};

#endif //PROJET_CPP_MAINVIEW_H

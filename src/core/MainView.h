#ifndef PROJET_CPP_MAINVIEW_H
#define PROJET_CPP_MAINVIEW_H

#include <QGraphicsView>

class MainView : public QGraphicsView {
    Q_OBJECT
    public:
        MainView(QWidget* parent = nullptr);
        virtual ~MainView();

        //Setters
        void setFitView(bool newFitView) { fitView = newFitView; }

        void deleteDeathScreen();

    protected:
        virtual void resizeEvent (QResizeEvent* event);
    private:
        bool fitView = true;
        QWidget* deathScreen = nullptr;

    public slots:
        void displayDeathScreen();


signals:
        void startGameRequested();
        void goToStartMenuRequested();
};

#endif //PROJET_CPP_MAINVIEW_H

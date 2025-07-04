#ifndef PROJET_CPP_MAINVIEW_H
#define PROJET_CPP_MAINVIEW_H

#include <QGraphicsView>
#include <QWheelEvent>
#include "ScoreManager.h"
#include "main_menu/SettingsWidget.h"

class MainWindow;

class MainView : public QGraphicsView {
    Q_OBJECT
    public:
        //Constructor and Destructor
        MainView(MainWindow* mainWindow, ScoreManager* scoreManager, QWidget* parent = nullptr);
        virtual ~MainView();

        //Setters
        void setFitView(bool newFitView) { fitView = newFitView; }

        void updatePauseScreenSize(QSize size);
        void createContentPauseContainer();

        //Delete
        void deleteDeathScreen();
        void deleteWinScreen();

        //In public to be able to call it from Main Window resizeEvent
        void resizeEvent (QResizeEvent* event) override;


protected:
        //Stop the wheel event for avoid being able to scroll on the view
        void wheelEvent(QWheelEvent* event) override { event->ignore(); }

    private:
        bool fitView = true;
        QWidget* deathScreen = nullptr;
        QWidget* winScreen = nullptr;
        QWidget* pauseScreen = nullptr;
        ScoreManager* scoreManager = nullptr;
        MainWindow* mainWindow = nullptr;

        QWidget* blackBackground = nullptr;
        SettingsWidget* settingsWidget = nullptr;
        QWidget* contentContainer = nullptr;

        QSize lastSize;

    public slots:
        void displayDeathScreen();
        void displayWinScreen();
        void displayPauseMenu();
        void stopGamePaused();
        void backToMenu();


signals:
        void startGameRequested();
        void goToStartMenuRequested();
};

#endif //PROJET_CPP_MAINVIEW_H

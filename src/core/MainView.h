#ifndef PROJET_CPP_MAINVIEW_H
#define PROJET_CPP_MAINVIEW_H

#include <QGraphicsView>

class MainView : public QGraphicsView {
    Q_OBJECT
    public:
        MainView(QWidget* parent = nullptr);
        virtual ~MainView();
    protected:
        virtual void resizeEvent (QResizeEvent* event){
            this->fitInView(sceneRect());
        }
};

#endif //PROJET_CPP_MAINVIEW_H

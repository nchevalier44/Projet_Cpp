#include "MainView.h"

MainView::MainView(QWidget* parent) : QGraphicsView(parent) {}

MainView::~MainView() {}

void MainView::resizeEvent (QResizeEvent* event){
    QGraphicsView::resizeEvent(event);

    //We want to have the dimension of the view equal to the scene only in the menu but no in game
    if(fit_view) {
        this->fitInView(sceneRect(), Qt::KeepAspectRatio);
    }
}
#ifndef PROJET_CPP_MAINVIEW_H
#define PROJET_CPP_MAINVIEW_H

#include <QGraphicsView>

class MainView : public QGraphicsView {
    Q_OBJECT
    public:
        MainView(QWidget* parent = nullptr);
        virtual ~MainView();

        //Getters
        bool getFitView() const { return fit_view; }

        //Setters
        void setFitView(bool new_fit_view) { fit_view = new_fit_view; }

    protected:
        virtual void resizeEvent (QResizeEvent* event);
    private:
        bool fit_view = true;
};

#endif //PROJET_CPP_MAINVIEW_H

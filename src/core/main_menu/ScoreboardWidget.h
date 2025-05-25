#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <QResizeEvent>
#include <QGraphicsProxyWidget>
#include <QLabel>
#include "../../constants.h"
#include "../FontManager.h"

#ifndef PROJET_CPP_SCOREBOARDWIDGET_H
#define PROJET_CPP_SCOREBOARDWIDGET_H

class MainWindow;


class ScoreboardWidget : public QWidget {
Q_OBJECT
public:
    //Constructor and destructor
    ScoreboardWidget(MainWindow *mainWindow, QWidget *parent = nullptr);

    ~ScoreboardWidget();

private:
    QPixmap* backgroundPixmap = nullptr;
    float ratioSettingsPixmap = 1;
    QWidget* scoresContainer = nullptr;

protected:
    //Redifined functions
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void mousePressEvent(QMouseEvent *event) override;

};

#endif //PROJET_CPP_SCOREBOARDWIDGET_H

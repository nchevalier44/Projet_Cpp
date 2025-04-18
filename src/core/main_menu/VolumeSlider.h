#ifndef PROJET_CPP_VOLUMESLIDER_H
#define PROJET_CPP_VOLUMESLIDER_H

#include <QPixmap>
#include <QSlider>
#include <QPainter>
#include <QResizeEvent>

#include "../../constants.h"


class VolumeSlider : public QSlider{
Q_OBJECT

public:
    //Constructor and destructor
    VolumeSlider(Qt::Orientation orientation, QWidget* parent=nullptr);
    virtual ~VolumeSlider();

private:
    double ratioHandle = 1;
    double ratioBar = 1;

    QPixmap* barPixmap = nullptr;
    QPixmap* handlePixmap = nullptr;

protected:
    //Redifined functions
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
};

#endif //PROJET_CPP_VOLUMESLIDER_H

#ifndef PROJET_CPP_MAINMENUBUTTON_H
#define PROJET_CPP_MAINMENUBUTTON_H

#include <QPushButton>

class MainMenuButton : public QPushButton{
    Q_OBJECT
public:
    MainMenuButton(QString text, QWidget* parent=nullptr);
    virtual ~MainMenuButton();

protected:
    void paintEvent(QPaintEvent* event) override;
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    QMovie* hoverMovie = nullptr;
    bool hover = false;
    QPixmap* backgroundPixmap = nullptr;
};


#endif //PROJET_CPP_MAINMENUBUTTON_H

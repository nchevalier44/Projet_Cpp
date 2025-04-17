#ifndef PROJET_CPP_WINDOWSIZECOMBOBOX_H
#define PROJET_CPP_WINDOWSIZECOMBOBOX_H

#include <QStyledItemDelegate>
#include <QComboBox>
#include <QPainter>

#include "../constants.h"

class WindowSizeComboBox : public QComboBox{
Q_OBJECT
public:
    //Constructor
    WindowSizeComboBox(QWidget* parent);
    virtual ~WindowSizeComboBox();

    //Getters
    float getRatioItem() const { return ratioItem; };
    QPixmap* getBackgroundPixmap() const { return backgroundPixmap; }

private:
    float ratioItem = 1;
    float ratioArrow = 1;
    QPixmap* backgroundPixmap = nullptr;
    QPixmap* arrowPixmap = nullptr;

protected:
    //Function redefined
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;


    };

class ComboBoxItemDelegate : public QStyledItemDelegate {
public:
    ComboBoxItemDelegate(WindowSizeComboBox* parent=nullptr);
    virtual ~ComboBoxItemDelegate();

private:
    QPixmap* backgroundPixmap = nullptr;
    float ratioItem = 1;
    WindowSizeComboBox* parent = nullptr;

protected:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};




#endif //PROJET_CPP_WINDOWSIZECOMBOBOX_H

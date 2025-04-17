#ifndef PROJET_CPP_WINDOWSIZECOMBOBOX_H
#define PROJET_CPP_WINDOWSIZECOMBOBOX_H

#include <QStyledItemDelegate>
#include <QComboBox>
#include <QPainter>

class WindowSizeComboBox : public QComboBox{
Q_OBJECT
public:
    WindowSizeComboBox(QWidget* parent);

    float getRatioItem() const { return ratioItem; };
    QPixmap* getBackgroundPixmap() const { return backgroundPixmap; }

private:
    float ratioItem = 1;
    float ratioArrow = 1;
    QPixmap* backgroundPixmap = nullptr;
    QPixmap* arrowPixmap = nullptr;
protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;


    };

class ComboBoxItemDelegate : public QStyledItemDelegate {
public:
    ComboBoxItemDelegate(WindowSizeComboBox* parent=nullptr);

private:
    QPixmap* backgroundPixmap = nullptr;
    float ratioItem = 1;
    WindowSizeComboBox* parent = nullptr;

protected:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};




#endif //PROJET_CPP_WINDOWSIZECOMBOBOX_H

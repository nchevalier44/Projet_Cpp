#include "WindowSizeComboBox.h"
#include <QResizeEvent>
#include <QAbstractItemView>
#include <QTimer>
#include <QFontDatabase>

ComboBoxItemDelegate::ComboBoxItemDelegate(WindowSizeComboBox* parent) : QStyledItemDelegate(parent) {
    backgroundPixmap = parent->getBackgroundPixmap();
    ratioItem = parent->getRatioItem();
    this->parent = parent;

}

QSize ComboBoxItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const {
    int itemWidth = parent->width();
    int itemHeight = int(itemWidth / ratioItem);
    return QSize(itemWidth, itemHeight);
}

void ComboBoxItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    painter->save();

    painter->drawPixmap(option.rect, backgroundPixmap->scaled(option.rect.size()));

    if (option.state & QStyle::State_Selected) {
        QPixmap scaled = backgroundPixmap->scaled(option.rect.size());

        QBitmap alphaMask = scaled.createMaskFromColor(Qt::transparent, Qt::MaskInColor);

        QRegion clipRegion(alphaMask);
        clipRegion.translate(option.rect.topLeft());

        painter->save();
        painter->setClipRegion(clipRegion);
        painter->fillRect(option.rect, QColor(255, 255, 255, 80));
        painter->restore();
    }

    QString text = index.data(Qt::DisplayRole).toString();
    painter->setPen(Qt::white);
    int fontId = QFontDatabase::addApplicationFont("../assets/fonts/Jersey10-Regular.ttf");
    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    painter->setFont(QFont(fontFamily, 0.4*option.rect.height()));
    painter->drawText(option.rect.adjusted(option.rect.width() * 0.1, 0, -option.rect.width() * 0.23, 0), Qt::AlignCenter, text);

    painter->restore();

}

WindowSizeComboBox::WindowSizeComboBox(QWidget* parent) : QComboBox(parent) {
    backgroundPixmap = new QPixmap("../assets/images/menu/dropdown_item.png");
    arrowPixmap = new QPixmap("../assets/images/menu/dropdown_arrow.png");
    ratioItem = float(backgroundPixmap->width()) / float(backgroundPixmap->height());
    ratioArrow = float(arrowPixmap->width()) / float(arrowPixmap->height());
    if (this->view()->parentWidget()){
        this->view()->parentWidget()->setStyleSheet("background: transparent;");
        this->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        this->view()->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    }
}




void WindowSizeComboBox::paintEvent(QPaintEvent* event) {
    QPainter painter(this);

    painter.drawPixmap(0, 0, backgroundPixmap->scaled(QSize(width(), height())));

    int arrowWidth = height() * 0.33 * ratioArrow;
    painter.drawPixmap(width() * 0.79, 0.33 * height(), arrowPixmap->scaled(arrowWidth, height()*0.33));

    QString text = currentText();
    painter.setPen(Qt::white);
    int fontId = QFontDatabase::addApplicationFont("../assets/fonts/Jersey10-Regular.ttf");
    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    painter.setFont(QFont(fontFamily, 0.4*height()));
    painter.drawText(rect().adjusted(width() * 0.1, 0, -width() * 0.23, 0), Qt::AlignCenter, text);

}

void WindowSizeComboBox::resizeEvent(QResizeEvent* event) {
    QComboBox::resizeEvent(event);
    int newHeight = float(event->size().width()) / float(ratioItem);
    if (height() != newHeight) {
        setFixedHeight(newHeight);
    }
}
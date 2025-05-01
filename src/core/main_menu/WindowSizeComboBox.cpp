#include "WindowSizeComboBox.h"
#include "../MainWindow.h"
#include "../FontManager.h"


//=====================================================
//==============WindowSizeComboBox=====================
//=====================================================

//Constructor
WindowSizeComboBox::WindowSizeComboBox(QWidget* parent) : QComboBox(parent) {
    //Setup pixmap and ratio
    backgroundPixmap = new QPixmap(PATH_MAIN_MENU_COMBOBOX_BACKGROUND);
    arrowPixmap = new QPixmap(PATH_MAIN_MENU_COMBOBOX_ARROW);
    ratioItem = float(backgroundPixmap->width()) / float(backgroundPixmap->height());
    ratioArrow = float(arrowPixmap->width()) / float(arrowPixmap->height());

    //Setup view attribute
    if (this->view()->parentWidget()){
        this->view()->parentWidget()->setStyleSheet("background: transparent;");
        this->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        this->view()->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    }
}

//Destructor
WindowSizeComboBox::~WindowSizeComboBox() {
    delete backgroundPixmap;
    backgroundPixmap = nullptr;
    delete arrowPixmap;
    arrowPixmap = nullptr;
}

//Redefinition of the paintEvent function to draw the background image and the arrow
void WindowSizeComboBox::paintEvent(QPaintEvent* event) {
    QPainter painter(this);

    //Draw the background
    painter.drawPixmap(0, 0, backgroundPixmap->scaled(QSize(width(), height())));

    //Draw the arrow with the right ratio
    int arrowHeight = height() * 0.33;
    int arrowWidth = arrowHeight * ratioArrow;

    //The arrow is put at 79% of the width of the comboBox (for the right position)
    painter.drawPixmap(width() * 0.79, arrowHeight, arrowPixmap->scaled(arrowWidth, arrowHeight));

    //Draw the text
    QString text = currentText();
    painter.setPen(Qt::white);

    //Font
    painter.setFont(QFont(FontManager::fontFamily, 0.4*height()));

    //The text is put at 10% of the width as far as 77% (to put it at the right position)
    painter.drawText(rect().adjusted(width() * 0.1, 0, -width() * 0.23, 0), Qt::AlignCenter, text);

}

//Redefinition of the resizeEvent function to resize the comboBox with the right ratio
void WindowSizeComboBox::resizeEvent(QResizeEvent* event) {
    QComboBox::resizeEvent(event);
    int newHeight = float(event->size().width()) / float(ratioItem);
    if (height() != newHeight) {
        setFixedHeight(newHeight);
    }
}



//=======================================================
//==============ComboBoxItemDelegate=====================
//=======================================================

//Constructor
ComboBoxItemDelegate::ComboBoxItemDelegate(WindowSizeComboBox* parent) : QStyledItemDelegate(parent) {
    //Get the pixmap from the ComboBox parent
    backgroundPixmap = parent->getBackgroundPixmap();
    ratioItem = parent->getRatioItem();
    this->parent = parent;
}

//Destructor
ComboBoxItemDelegate::~ComboBoxItemDelegate(){}

//Redefinition of the sizeHint function to resize the item with the right ratio (resizeEvent doesn't exist in QStyledItemDelegate)
QSize ComboBoxItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const {
    int itemWidth = parent->width();
    int itemHeight = int(itemWidth / ratioItem);
    return QSize(itemWidth, itemHeight);
}

//Redefinition of the paint function to draw the background image and the text with the right ratio
void ComboBoxItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    //Save the state of the painter (color, transformation, ...)
    painter->save();
    QRect rect = option.rect;
    //Draw the background
    painter->drawPixmap(rect, backgroundPixmap->scaled(rect.size()));

    // '&' operator because option can have multiple state like QStyle::State_Selected and QStyle::State_Enabled
    //so we can't use '==' operator
    if (option.state & QStyle::State_Selected) {
        //Add semi-transparent filter to selected item

        QPixmap scaled = backgroundPixmap->scaled(rect.size());

        //Create a mask from the transparent pixels
        QBitmap alphaMask = scaled.createMaskFromColor(Qt::transparent, Qt::MaskInColor);

        //Clip to remove the transparent pixels (mask) from the region
        QRegion clipRegion(alphaMask);
        clipRegion.translate(rect.topLeft());

        painter->setClipRegion(clipRegion);
        painter->fillRect(rect, QColor(255, 255, 255, 80)); //Fill with semi-transparent white
    }

    //Get the text
    QString text = index.data(Qt::DisplayRole).toString();
    painter->setPen(Qt::white);

    //Font
    painter->setFont(QFont(FontManager::fontFamily, 0.4*rect.height()));

    //The text is put at 10% of the width as far as 77% (to put it at the right position)
    painter->drawText(rect.adjusted(rect.width() * 0.1, 0, -rect.width() * 0.23, 0), Qt::AlignCenter, text);

    //Restore the state that we have saved before
    painter->restore();
}
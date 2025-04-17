#include "SettingsWidget.h"
#include "MainWindow.h"

SettingsWidget::SettingsWidget(MainWindow* mainWindow, QWidget* parent) : QWidget(parent) {

    this->settingsBackgroundPixmap = new QPixmap("../assets/images/menu/settings_background.png");
    this->ratioSettingsPixmap = float(this->settingsBackgroundPixmap->width()) / float(this->settingsBackgroundPixmap->height());

    int fontId = QFontDatabase::addApplicationFont("../assets/fonts/Jersey10-Regular.ttf");
    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont textFont(fontFamily, 20);
    QFont titleFont(fontFamily, 40);

    //Title
    QLabel* title = new QLabel("Settings", this);
    title->setStyleSheet("color: white;");
    title->setAlignment(Qt::AlignCenter);
    title->setFont(titleFont);

    //Volume widgets (slider) and layout
    QWidget* volumeMusicWidget = new QWidget(this);
    QHBoxLayout* volumeMusicLayout = new QHBoxLayout(volumeMusicWidget);
    QLabel* volumeMusicLabel = new QLabel("Music Volume", volumeMusicWidget);
    volumeMusicLabel->setStyleSheet("color: white;");
    volumeMusicLabel->setFont(textFont);
    volumeMusicSlider = new VolumeSlider(Qt::Horizontal, volumeMusicWidget);
    volumeMusicSlider->setValue(mainWindow->getVolumePercentage());
    volumeMusicLayout->setSpacing(0.1 * width());
    volumeMusicLayout->addWidget(volumeMusicLabel);
    volumeMusicLayout->addWidget(volumeMusicSlider);
    volumeMusicWidget->setLayout(volumeMusicLayout);

    //Volume widgets (slider) and layout
    QWidget* volumeSFXWidget = new QWidget(this);
    QHBoxLayout* volumeSFXLayout = new QHBoxLayout(volumeSFXWidget);
    QLabel* volumeLabel = new QLabel("Sounds effects\nvolume", volumeSFXWidget);
    volumeLabel->setStyleSheet("color: white;");
    volumeLabel->setAlignment(Qt::AlignCenter);
    volumeLabel->setFont(textFont);
    volumeSFXSlider = new VolumeSlider(Qt::Horizontal, volumeSFXWidget);
    volumeSFXSlider->setValue(mainWindow->getVolumePercentage());
    volumeSFXLayout->setSpacing(0.1 * width());
    volumeSFXLayout->addWidget(volumeLabel);
    volumeSFXLayout->addWidget(volumeSFXSlider);
    volumeSFXWidget->setLayout(volumeSFXLayout);

    //Window size widgets (dropdown) and layout
    QWidget* windowSizeWidget = new QWidget(this);
    QHBoxLayout* windowSizeLayout = new QHBoxLayout(windowSizeWidget);
    QLabel* windowSizeLabel = new QLabel("Window size", windowSizeWidget);
    windowSizeLabel->setStyleSheet("color: white;");
    windowSizeLabel->setFont(textFont);
    windowSizeComboBox = new WindowSizeComboBox(windowSizeWidget);
    windowSizeComboBox->setItemDelegate(new ComboBoxItemDelegate(windowSizeComboBox));
    for (int h : {1440, 1080, 720, 480} ){
        windowSizeComboBox->addItem(QString::number(mainWindow->getBackgroundRatio() * h) + "x" + QString::number(h));
        qDebug() << mainWindow->getBackgroundRatio();
    }
    windowSizeComboBox->setCurrentText(QString::number(mainWindow->getBackgroundRatio() * 1080) + "x" + QString::number(1080));
    windowSizeComboBox->addItem("Custom");
    windowSizeLayout->setSpacing(0.25 * width());
    windowSizeLayout->setAlignment(Qt::AlignCenter);
    windowSizeLayout->addWidget(windowSizeLabel);
    windowSizeLayout->addWidget(windowSizeComboBox);
    windowSizeWidget->setLayout(windowSizeLayout);

    connect(windowSizeComboBox, &QComboBox::currentTextChanged, this, [=](const QString &text){
        qDebug() << "RATIO : " << float(this->width()) << float(this->height());
        for (int h : {1440, 1080, 720, 480} ){
            QString windowSize = QString::number(mainWindow->getBackgroundRatio() * h) + "x" + QString::number(h);
            qDebug() << text << windowSize << mainWindow->getBackgroundRatio();
            if(text == windowSize){
                mainWindow->setFixedSize(mainWindow->getBackgroundRatio() * h, h);
            } else if(text == "Custom"){
                mainWindow->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
            }
        }
    });

    //Button (cancel and save) widgets and layout
    MainMenuButton* closeButton = new MainMenuButton("Close", this);
    closeButton->setFont(textFont);
    QHBoxLayout* closeButtonLayout = new QHBoxLayout();
    closeButtonLayout->addWidget(closeButton);
    QObject::connect(closeButton, &QPushButton::clicked, this, &SettingsWidget::hide);

    //Container layout (of SettingsWidget)
    QVBoxLayout* containerLayout = new QVBoxLayout(this);
    containerLayout->setAlignment(Qt::AlignCenter);
    containerLayout->addWidget(title);
    containerLayout->addWidget(volumeMusicWidget);
    containerLayout->addWidget(volumeSFXWidget);
    containerLayout->addWidget(windowSizeWidget);
    containerLayout->addLayout(closeButtonLayout);
    this->setLayout(containerLayout);

    this->setContentsMargins(width() * 0.1, width() * 0.1, width() * 0.1, width() * 0.1);
    closeButton->setFixedWidth(width() * 0.1);
    windowSizeComboBox->setFixedWidth(width() * 0.45);
    volumeMusicSlider->setFixedWidth(width() * 0.6);
    volumeSFXSlider->setFixedWidth(width() * 0.6);
    volumeMusicSlider->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    volumeSFXSlider->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    containerLayout->setAlignment(Qt::AlignHCenter);

    this->setStyleSheet("background: transparent;");
}

void SettingsWidget::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    QPainterPath path;
    path.addRoundedRect(rect(), width() * 0.1, height() * 0.1);

    painter.setOpacity(0.8);
    painter.setPen(Qt::NoPen); //No border
    painter.fillPath(path, settingsBackgroundPixmap->scaled(width(), width() / ratioSettingsPixmap)); // fond
    painter.drawPath(path);
}

void SettingsWidget::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
    QSize newSize = event->size();
    this->setFixedSize(newSize.width(), newSize.width() / ratioSettingsPixmap);
}

VolumeSlider::VolumeSlider(Qt::Orientation orientation, QWidget* parent) : QSlider(orientation, parent){
    this->setMinimum(0);
    this->setMaximum(100);
    barPixmap = new QPixmap("../assets/images/menu/slider_bar.png");
    handlePixmap = new QPixmap("../assets/images/menu/slider_handle.png");
    ratioHandle = float(handlePixmap->width()) / float(handlePixmap->height());
    ratioBar = float(barPixmap->width()) / float(barPixmap->height());
}


void VolumeSlider::paintEvent(QPaintEvent* event) {
    QSlider::paintEvent(event);
    QPainter painter(this);

    float newHandleWidth = ratioHandle * height();

    //Draw the bar of the slider (all the width)
    painter.drawPixmap(0, 0, barPixmap->scaled( QSize(width(), width() / ratioBar)));;

    //Draw the handle of the slider
    float ratioPosition = float(value() - minimum()) / float(maximum() - minimum());
    painter.drawPixmap(ratioPosition * (width() - newHandleWidth), 0, handlePixmap->scaled( QSize(newHandleWidth, height())));
    qDebug() << this->value();


}

void VolumeSlider::resizeEvent(QResizeEvent* event) {
    QSlider::resizeEvent(event);
    int newHeight = float(event->size().width()) / float(ratioBar);
    if (height() != newHeight) {
        setFixedHeight(newHeight);
    }
}
#include "SettingsWidget.h"
#include "../MainWindow.h"
#include "../FontManager.h"

//Constructor
SettingsWidget::SettingsWidget(MainWindow* mainWindow, QWidget* parent) : QWidget(parent) {

    //Setup pixmap, ratio and background transparence
    this->settingsBackgroundPixmap = new QPixmap(PATH_MAIN_MENU_SETTINGS_BACKGROUND);
    this->ratioSettingsPixmap = float(this->settingsBackgroundPixmap->width()) / float(this->settingsBackgroundPixmap->height());
    this->setStyleSheet("background: transparent;");

    //Font
    textFont.setFamily(FontManager::fontFamily);
    titleFont.setFamily(FontManager::fontFamily);
    QString percentageText;
    
    //Title
    title = new QLabel("Settings", this);
    title->setStyleSheet("color: white;");
    title->setAlignment(Qt::AlignCenter);



    //Volume slider music widget
    QWidget* volumeMusicWidget = new QWidget(this);
    volumeMusicSlider = new VolumeSlider(Qt::Horizontal, volumeMusicWidget);
    volumeMusicSlider->setValue(mainWindow->getAudioManager()->getMusicVolumePercentage());

    //Volume music labels
    volumeMusicLabel = new QLabel("Music Volume", volumeMusicWidget);
    volumeMusicLabel->setStyleSheet("color: white;");
    volumeMusicLabel->setAlignment(Qt::AlignCenter);
    percentageText = QString::number(mainWindow->getAudioManager()->getMusicVolumePercentage()) + " %";
    volumeMusicValueLabel = new QLabel(percentageText, volumeMusicWidget);
    volumeMusicValueLabel->setStyleSheet("color: white;");

    //Volume music layout
    volumeMusicLayout = new QHBoxLayout(volumeMusicWidget);
    volumeMusicLayout->addWidget(volumeMusicLabel);
    volumeMusicLayout->addWidget(volumeMusicSlider);
    volumeMusicLayout->addWidget(volumeMusicValueLabel);


    
    //Volume SFX slider widget
    QWidget* volumeSFXWidget = new QWidget(this);
    volumeSFXSlider = new VolumeSlider(Qt::Horizontal, volumeSFXWidget);
    volumeSFXSlider->setValue(mainWindow->getAudioManager()->getSFXVolumePercentage());

    //Volume SFX labels
    volumeSFXLabel = new QLabel("Sounds effects\nvolume", volumeSFXWidget);
    volumeSFXLabel->setStyleSheet("color: white;");
    volumeSFXLabel->setAlignment(Qt::AlignCenter);

    percentageText = QString::number(mainWindow->getAudioManager()->getSFXVolumePercentage()) + " %";
    volumeSFXValueLabel = new QLabel(percentageText, volumeSFXWidget);
    volumeSFXValueLabel->setStyleSheet("color: white;");


    //Volume SFX layout
    volumeSFXLayout = new QHBoxLayout(volumeSFXWidget);
    volumeSFXLayout->addWidget(volumeSFXLabel);
    volumeSFXLayout->addWidget(volumeSFXSlider);
    volumeSFXLayout->addWidget(volumeSFXValueLabel);

    //Actions of the sliders
    connect(volumeMusicSlider, &VolumeSlider::valueChanged, this, [=](int value){
        volumeMusicValueLabel->setText(QString::number(value) + " %");
        mainWindow->getAudioManager()->setMusicVolumePercentage(value);
        mainWindow->getAudioManager()->updateMusicVolume();
    });
    connect(volumeSFXSlider, &VolumeSlider::valueChanged, this, [=](int value){
        volumeSFXValueLabel->setText(QString::number(value) + " %");
        mainWindow->getAudioManager()->setSFXVolumePercentage(value);
        mainWindow->getAudioManager()->updateSFXVolume();
    });




    //Window size combobox widget
    QWidget* windowSizeWidget = new QWidget(this);
    windowSizeComboBox = new WindowSizeComboBox(windowSizeWidget);
    windowSizeComboBox->setItemDelegate(new ComboBoxItemDelegate(windowSizeComboBox));
    float mainWindowBackgroundRatio = mainWindow->getBackgroundRatio();

    for (int h : LIST_WINDOW_HEIGHT ){
        windowSizeComboBox->addItem(QString::number(mainWindowBackgroundRatio * h) + "x" + QString::number(h));
    }
    windowSizeComboBox->setCurrentText(QString::number(mainWindowBackgroundRatio * mainWindow->height()) + "x" + QString::number(mainWindow->height()));
    windowSizeComboBox->addItem("Custom");

    //Window size Label
    windowSizeLabel = new QLabel("Window size", windowSizeWidget);
    windowSizeLabel->setStyleSheet("color: white;");
    windowSizeLabel->setAlignment(Qt::AlignCenter);

    //Window size layout
    windowSizeLayout = new QHBoxLayout(windowSizeWidget);
    windowSizeLayout->setAlignment(Qt::AlignCenter);
    windowSizeLayout->addWidget(windowSizeLabel);
    windowSizeLayout->addWidget(windowSizeComboBox);

    //Apply change of window size combobox
    connect(windowSizeComboBox, &QComboBox::currentTextChanged, this, [=](const QString &text){
        for (int h : LIST_WINDOW_HEIGHT ){
            int newWidth = mainWindowBackgroundRatio * h;
            QString windowSize = QString::number(newWidth) + "x" + QString::number(h);
            
            if(text == windowSize){
                QResizeEvent re(QSize(newWidth, h), QSize(mainWindow->width(), mainWindow->height()));
                mainWindow->setFixedSize(newWidth, h);
                mainWindow->getMainView()->resizeEvent(&re);

            } else if(text == "Custom"){
                mainWindow->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
                mainWindow->setMinimumSize(QSize(0, 0));
                mainWindow->setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
            }
        }
    });


    //Close button
    closeButton = new MainMenuButton("Close", mainWindow->getAudioManager(), this);
    QHBoxLayout* closeButtonLayout = new QHBoxLayout();
    closeButtonLayout->addWidget(closeButton);
    QObject::connect(closeButton, &QPushButton::clicked, this, &SettingsWidget::hide);


    //Container layout (of SettingsWidget)
    containerLayout = new QVBoxLayout(this);
    containerLayout->setAlignment(Qt::AlignCenter);
    containerLayout->addWidget(title);
    containerLayout->addWidget(volumeMusicWidget);
    containerLayout->addWidget(volumeSFXWidget);
    containerLayout->addWidget(windowSizeWidget);
    containerLayout->addLayout(closeButtonLayout);

    //Set the right size for all items here
    resizeItems(width(), height());
    volumeMusicSlider->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    volumeSFXSlider->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    volumeSFXLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    volumeMusicLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    //Set attribute so that buttons located below cannot be clicked on
    this->setAttribute(Qt::WA_TransparentForMouseEvents, false);
}


//Destructor
SettingsWidget::~SettingsWidget() {
    delete settingsBackgroundPixmap;
    settingsBackgroundPixmap = nullptr;
}

//Redefinition of the paintEvent function to draw the background with the right ratio
void SettingsWidget::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    QPainterPath path;
    path.addRoundedRect(rect(), width() * 0.1, height() * 0.1); //Rounded corners

    if(settingsBackgroundPixmap && !settingsBackgroundPixmap->isNull()){
        painter.setOpacity(0.8);
        painter.setPen(Qt::NoPen); //No border
        painter.fillPath(path, settingsBackgroundPixmap->scaled(width(), width() / ratioSettingsPixmap));
        painter.drawPath(path);
    }

}

//Redefinition of the resizeEvent function to resize the widget with the right ratio
void SettingsWidget::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
    QSize newSize = event->size();
    this->setFixedSize(newSize.width(), newSize.width() / ratioSettingsPixmap);
    resizeItems(newSize.width(), newSize.width() / ratioSettingsPixmap); //Resize other items in the widget
}


//Function that resize all items in settings widget in function of the width and height given
void SettingsWidget::resizeItems(int width, int height){
    windowSizeComboBox->setFixedWidth(width * 0.40);
    volumeMusicSlider->setFixedWidth(width * 0.425);
    volumeSFXSlider->setFixedWidth(width * 0.425);
    closeButton->setFixedSize(QSize(width * 0.35, height * 0.09));

    textFont.setPixelSize(height * 0.04);
    titleFont.setPixelSize(height * 0.08);
    title->setFont(titleFont);
    volumeMusicLabel->setFont(textFont);
    volumeMusicValueLabel->setFont(textFont);
    volumeSFXLabel->setFont(textFont);
    volumeSFXValueLabel->setFont(textFont);
    windowSizeLabel->setFont(textFont);
    closeButton->setFont(textFont);

    volumeSFXLabel->adjustSize(); //Force the size to be updated before setting the fixed width
    volumeMusicLabel->setFixedWidth(volumeSFXLabel->width());


    volumeMusicLayout->setSpacing(0.015 * width);
    volumeSFXLayout->setSpacing(0.015 * width);
    windowSizeLayout->setSpacing(0.12 * width);
    containerLayout->setSpacing(0.05 * height);
    windowSizeLayout->activate();
    volumeMusicLayout->activate();
    volumeSFXLayout->activate();
    containerLayout->activate();
}

void SettingsWidget::mousePressEvent(QMouseEvent *event) {
    event->accept();  //Stop the mouse event in order to not click on the button behind the widget
}
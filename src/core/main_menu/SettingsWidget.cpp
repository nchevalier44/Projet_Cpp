#include "SettingsWidget.h"
#include "../MainWindow.h"
#include "../FontManager.h"

//Constructor
SettingsWidget::SettingsWidget(MainWindow* mainWindow, QWidget* parent) : QWidget(parent) {

    //Setup pixmap, ratio and background transparence
    this->settingsBackgroundPixmap = new QPixmap(PATH_MAIN_MENU_SETTINGS_BACKGROUND);
    this->ratioSettingsPixmap = float(this->settingsBackgroundPixmap->width()) / float(this->settingsBackgroundPixmap->height());
    this->setStyleSheet("background: transparent;");
    this->setAttribute(Qt::WA_TransparentForMouseEvents, false);

    //Font
    QFont textFont(FontManager::fontFamily, 23);
    QFont titleFont(FontManager::fontFamily, 40);

    
    //Title
    QLabel* title = new QLabel("Settings", this);
    title->setStyleSheet("color: white;");
    title->setAlignment(Qt::AlignCenter);
    title->setFont(titleFont);
    


    //Volume slider music widget
    QWidget* volumeMusicWidget = new QWidget(this);
    volumeMusicSlider = new VolumeSlider(Qt::Horizontal, volumeMusicWidget);
    volumeMusicSlider->setValue(mainWindow->getAudioManager()->getMusicVolumePercentage());

    //Volume music labels
    QLabel* volumeMusicLabel = new QLabel("Music Volume", volumeMusicWidget);
    volumeMusicLabel->setStyleSheet("color: white;");
    volumeMusicLabel->setFont(textFont);
    volumeMusicLabel->setAlignment(Qt::AlignCenter);
    QLabel* volumeMusicValueLabel = new QLabel("100 %", volumeMusicWidget);
    volumeMusicValueLabel->setStyleSheet("color: white;");
    volumeMusicValueLabel->setFont(textFont);

    //Volume music layout
    QHBoxLayout* volumeMusicLayout = new QHBoxLayout(volumeMusicWidget);
    volumeMusicLayout->setSpacing(0.05 * width());
    volumeMusicLayout->addWidget(volumeMusicLabel);
    volumeMusicLayout->addWidget(volumeMusicSlider);
    volumeMusicLayout->addWidget(volumeMusicValueLabel);


    
    //Volume SFX slider widget
    QWidget* volumeSFXWidget = new QWidget(this);
    volumeSFXSlider = new VolumeSlider(Qt::Horizontal, volumeSFXWidget);
    volumeSFXSlider->setValue(mainWindow->getAudioManager()->getSFXVolumePercentage());

    //Volume SFX labels
    QLabel* volumeSFXLabel = new QLabel("Sounds effects\nvolume", volumeSFXWidget);
    volumeSFXLabel->setStyleSheet("color: white;");
    volumeSFXLabel->setAlignment(Qt::AlignCenter);
    volumeSFXLabel->setFont(textFont);
    QLabel* volumeSFXValueLabel = new QLabel("100 %", volumeSFXWidget);
    volumeSFXValueLabel->setStyleSheet("color: white;");
    volumeSFXValueLabel->setFont(textFont);


    //Volume SFX layout
    QHBoxLayout* volumeSFXLayout = new QHBoxLayout(volumeSFXWidget);
    volumeSFXLayout->setSpacing(0.05 * width());
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
    windowSizeComboBox->setCurrentText(QString::number(mainWindowBackgroundRatio * DEFAULT_WINDOW_HEIGHT) + "x" + QString::number(DEFAULT_WINDOW_HEIGHT));
    windowSizeComboBox->addItem("Custom");

    //Window size Label
    QLabel* windowSizeLabel = new QLabel("Window size", windowSizeWidget);
    windowSizeLabel->setStyleSheet("color: white;");
    windowSizeLabel->setFont(textFont);
    windowSizeLabel->setAlignment(Qt::AlignCenter);

    //Window size layout
    QHBoxLayout* windowSizeLayout = new QHBoxLayout(windowSizeWidget);
    windowSizeLayout->setSpacing(0.2 * width());
    windowSizeLayout->setAlignment(Qt::AlignCenter);
    windowSizeLayout->addWidget(windowSizeLabel);
    windowSizeLayout->addWidget(windowSizeComboBox);

    //Apply change of window size combobox
    connect(windowSizeComboBox, &QComboBox::currentTextChanged, this, [=](const QString &text){
        for (int h : LIST_WINDOW_HEIGHT ){
            int newWidth = mainWindowBackgroundRatio * h;
            QString windowSize = QString::number(newWidth) + "x" + QString::number(h);
            
            if(text == windowSize){
                mainWindow->setFixedSize(newWidth, h);
            } else if(text == "Custom"){
                mainWindow->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
                mainWindow->setMinimumSize(QSize(0, 0));
                mainWindow->setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
            }
        }
    });



    //Close button
    MainMenuButton* closeButton = new MainMenuButton("Close", this);
    closeButton->setFont(textFont);
    QHBoxLayout* closeButtonLayout = new QHBoxLayout();
    closeButtonLayout->addWidget(closeButton);
    QObject::connect(closeButton, &QPushButton::clicked, this, &SettingsWidget::hide);




    //Container layout (of SettingsWidget)
    QVBoxLayout* containerLayout = new QVBoxLayout(this);
    containerLayout->setAlignment(Qt::AlignCenter);
    containerLayout->setSpacing(0.1 * height());
    containerLayout->addWidget(title);
    containerLayout->addWidget(volumeMusicWidget);
    containerLayout->addWidget(volumeSFXWidget);
    containerLayout->addWidget(windowSizeWidget);
    containerLayout->addLayout(closeButtonLayout);




    //Set margin
    int margin = width() * 0.1;
    this->setContentsMargins(width() * 0.12, margin, margin, margin);

    //Set the size of each widget (they will have the ratio of their image with resizeEvent redifined for each of them)
    volumeSFXLabel->adjustSize(); //Force the size to be updated before setting the fixed width
    volumeMusicLabel->setFixedWidth(volumeSFXLabel->width());

    closeButton->setFixedWidth(width() * 0.2);
    windowSizeComboBox->setFixedWidth(width() * 0.45);
    volumeMusicSlider->setFixedWidth(width() * 0.45);
    volumeSFXSlider->setFixedWidth(width() * 0.45);
    volumeMusicSlider->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    volumeSFXSlider->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

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
}
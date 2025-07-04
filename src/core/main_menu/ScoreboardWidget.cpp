#include <QtWidgets>
#include "ScoreboardWidget.h"
#include "../MainWindow.h"


ScoreboardWidget::ScoreboardWidget(MainWindow* mainWindow, QWidget* parent) : QWidget(parent) {

    //Setup pixmap, ratio and background transparence
    this->backgroundPixmap = new QPixmap(PATH_MAIN_MENU_SETTINGS_BACKGROUND);
    this->ratioSettingsPixmap = float(this->backgroundPixmap->width()) / float(this->backgroundPixmap->height());
    this->setStyleSheet("background: transparent;");

    //Font
    QFont textFont(FontManager::fontFamily, 23);
    QFont titleFont(FontManager::fontFamily, 40);
    QFont font(FontManager::fontFamily, 35);


    //Title
    QLabel* title = new QLabel("Scoreboard", this);
    title->setStyleSheet("color: white;");
    title->setAlignment(Qt::AlignCenter);
    title->setFont(titleFont);

    //Scores
    scoresContainer = new QWidget(this);
    QVBoxLayout* scoresLayout = new QVBoxLayout();

    //Get best scores
    mainWindow->getScoreManager()->loadScores();
    QList<Score> scores = mainWindow->getScoreManager()->getBestScoresList();
    int n = scores.size();

    if(n == 0){ // if no scores
        //it crash if when we decomment this (only when we start the first time and we click 'back to menu' when we die / win / pause)
        /*QLabel* label = new QLabel("No previous scores...");
        label->setFont(font);
        label->setStyleSheet("color: white;");
        scoresLayout->addWidget(label);*/
    } else{ //if scores exist display each scores
        for (int i = n - 1; i >= 0; i--) {
            QString number = QString::number(n - i);
            QString score = QString::number(scores[i].getScore());
            int seconds = scores[i].getTimePlayed();
            QString timePlayed = QString::number(seconds / 60) + "m" + QString::number(seconds % 60) + "s"; //convert seconds in minutes and seconds
            QString date = scores[i].getDate();

            QLabel* label = new QLabel(number + " | " + score + " pts - " + timePlayed + " - " + date);
            label->setFont(font);
            label->setStyleSheet("color: white;");
            scoresLayout->addWidget(label);
        }
    }
    scoresContainer->setLayout(scoresLayout);
    scoresContainer->setStyleSheet("background-color: transparent;");

   
    //Close button
    MainMenuButton* closeButton = new MainMenuButton("Close", mainWindow->getAudioManager(), this);
    closeButton->setFont(textFont);
    QHBoxLayout* closeButtonLayout = new QHBoxLayout();
    closeButtonLayout->addWidget(closeButton);
    QObject::connect(closeButton, &QPushButton::clicked, this, &ScoreboardWidget::hide);




    //Container layout (of ScoreboardWidget)
    QVBoxLayout* containerLayout = new QVBoxLayout(this);
    containerLayout->setAlignment(Qt::AlignCenter);
    containerLayout->setSpacing(0.1 * height());
    containerLayout->addWidget(title);
    containerLayout->addWidget(scoresContainer);
    containerLayout->addLayout(closeButtonLayout);




    //Set margin
    int margin = width() * 0.1;
    this->setContentsMargins(width() * 0.12, margin, margin, margin);

    closeButton->setFixedSize(closeButton->size() * 3);

    //Set attribute so that buttons located below cannot be clicked on
    this->setAttribute(Qt::WA_TransparentForMouseEvents, false);



}


//Destructor
ScoreboardWidget::~ScoreboardWidget(){
    if(backgroundPixmap){
        delete backgroundPixmap;
        backgroundPixmap = nullptr;
    }
    if(scoresContainer){
        delete scoresContainer;
        scoresContainer = nullptr;
    }
}





//Redefinition of the paintEvent function to draw the pixmap
void ScoreboardWidget::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    QPainterPath path;
    path.addRoundedRect(rect(), width() * 0.1, height() * 0.1); //Rounded corners

    if(backgroundPixmap && !backgroundPixmap->isNull()){
        painter.setOpacity(0.8);
        painter.setPen(Qt::NoPen); //No border
        painter.fillPath(path, backgroundPixmap->scaled(width(), width() / ratioSettingsPixmap));
        painter.drawPath(path);
    }
}

//Redefinition of the resizeEvent function to resize the widget with the right ratio
void ScoreboardWidget::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
    QSize newSize = event->size();
    this->setFixedSize(newSize.width(), newSize.width() / ratioSettingsPixmap);
}

void ScoreboardWidget::mousePressEvent(QMouseEvent *event) {
    event->accept();  //Stop the mouse event in order to not click on the button behind the widget
}
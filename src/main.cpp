#include <QApplication>
#include <iostream>
#include <string>

#include "core/MainWindow.h"
#include "./entities/Entity.h"


int main(int argc, char *argv[]) {
    QApplication application(argc, argv);
    MainWindow mainWindow;
    mainWindow.show();
    return application.exec();
}

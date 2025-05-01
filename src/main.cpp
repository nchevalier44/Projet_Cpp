#include <QApplication>

#include "core/MainWindow.h"
#include "core/FontManager.h"

int main(int argc, char *argv[]) {
    QApplication application(argc, argv);
    FontManager::loadFonts();
    MainWindow mainWindow;
    mainWindow.show();
    return application.exec();
}

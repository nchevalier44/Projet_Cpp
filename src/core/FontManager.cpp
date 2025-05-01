#include "FontManager.h"
#include "../constants.h"
#include <QFontDatabase>
#include <QDebug>

QString FontManager::fontFamily;
bool FontManager::isLoaded = false;

void FontManager::loadFonts() {
    if (!isLoaded) {
        int id = QFontDatabase::addApplicationFont(PATH_JERSEY10_FONT);
        if (id != -1) {
            fontFamily = QFontDatabase::applicationFontFamilies(id).at(0);
            isLoaded = true;
        } else {
            qWarning() << "Failed to load font";
        }
    }
}
#ifndef PROJET_CPP_FONTMANAGER_H
#define PROJET_CPP_FONTMANAGER_H

#include <QString>
#include <QFont>

class FontManager {
public:
    static void loadFonts();
    static QString fontFamily;
private:
    static bool isLoaded;
};


#endif //PROJET_CPP_FONTMANAGER_H

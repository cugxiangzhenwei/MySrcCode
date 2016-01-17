#include <QApplication>
#include <QTranslator>
#include "palette.h"

int main( int argc, char * argv[] )
{
    QApplication app(argc, argv);
    
    QTranslator translator;
    translator.load("palette_zh");
    app.installTranslator(&translator);

    Palette p;
    p.show();
    return app.exec();
}

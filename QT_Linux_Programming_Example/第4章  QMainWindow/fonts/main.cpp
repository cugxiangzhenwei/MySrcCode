#include <QApplication>
#include "fontset.h"

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);
    
    QTranslator translator;
    translator.load("fonts_zh");
    app.installTranslator(&translator);
    FontSet *t = new FontSet();
    t->show();
    return app.exec();
}

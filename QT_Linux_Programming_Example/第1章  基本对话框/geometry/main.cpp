#include "geometry.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QFont font("ZYSong18030",12);
    QApplication::setFont(font);
    	
    QApplication app(argc,argv);
    QTranslator translator(0);
    translator.load("geometry_zh",".");
    app.installTranslator(&translator);    
    Geometry *g = new Geometry();
    g->show();
    return app.exec();
}

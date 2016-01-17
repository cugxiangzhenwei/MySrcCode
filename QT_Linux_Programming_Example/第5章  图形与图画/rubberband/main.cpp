#include <QApplication>
#include "rubberband.h"

int
main(int argc, char * argv[])
{
    QApplication app(argc,argv);
    
    QTranslator translator;
    translator.load("rubberband_zh");
    app.installTranslator(&translator);
    
    RubberBand rubber;
    rubber.show();
    return app.exec();
}

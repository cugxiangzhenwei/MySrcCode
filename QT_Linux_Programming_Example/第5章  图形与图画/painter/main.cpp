#include <QApplication>
#include "painter.h"

int
main(int argc, char * argv[])
{
	
    QApplication app(argc,argv);
    
    QTranslator translator;
    translator.load("painter_zh");
    app.installTranslator(&translator);
    
    Painter p;
    p.show();
    return app.exec();
}

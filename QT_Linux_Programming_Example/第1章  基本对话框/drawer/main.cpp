#include "drawer.h"
#include <QApplication>


int main( int argc, char **argv )
{
    QFont font("ZYSong18030",10);
    QApplication::setFont(font);	
    
    QApplication a( argc, argv );
    QTranslator translator(0);
    translator.load("drawer_zh",".");
    a.installTranslator(&translator);
    
    Drawer *drawer = new Drawer();
    drawer->resize( 120, 360 );
    drawer->show();
    return a.exec();
}

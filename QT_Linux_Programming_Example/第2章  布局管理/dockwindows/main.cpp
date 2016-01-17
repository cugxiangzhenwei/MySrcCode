#include "dockwindows.h"
#include <QApplication>


int main( int argc, char **argv )
{
    QFont font("ZYSong18030",12);
    QApplication::setFont(font);
    	
    QApplication a( argc, argv );
    QTranslator translator(0);
    translator.load("dockwindows_zh",".");
    a.installTranslator(&translator);  

    DockWindows *dockwindows = new DockWindows();
    dockwindows->show();
    return a.exec();
}

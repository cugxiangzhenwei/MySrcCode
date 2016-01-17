#include "mouseevent.h"
#include <QApplication>


int main( int argc, char **argv )
{
    QFont font("ZYSong18030",12);
    QApplication::setFont(font);
    	
    QApplication a( argc, argv );
    QTranslator translator(0);
    translator.load("mouseevent_zh",".");
    a.installTranslator(&translator);    
    
    MouseEvent *mouseevent = new MouseEvent();
    mouseevent->show();
    return a.exec();
}

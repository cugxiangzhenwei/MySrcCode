#include "keyevent.h"
#include <QApplication>


int main( int argc, char **argv )
{
    QFont font("ZYSong18030",12);
    QApplication::setFont(font);
    	
    QApplication a( argc, argv );
    QTranslator translator(0);
    translator.load("keyevent_zh",".");
    a.installTranslator(&translator);    
    
    KeyEvent *keyevent = new KeyEvent();
    keyevent->show();
    return a.exec();
}

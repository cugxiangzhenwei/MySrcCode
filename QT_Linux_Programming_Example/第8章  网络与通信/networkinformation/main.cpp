#include "networkinformation.h"
#include <QApplication>


int main( int argc, char **argv )
{
    QFont font("ZYSong18030",12);
    QApplication::setFont(font);
    	
    QApplication a( argc, argv );
    QTranslator translator(0);
    translator.load("networkinformation_zh",".");
    a.installTranslator(&translator);    
    
    NetworkInformation *networkinformation = new NetworkInformation();
    networkinformation->show();
    return a.exec();
}

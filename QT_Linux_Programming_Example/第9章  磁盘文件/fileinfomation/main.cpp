#include "fileinformation.h"
#include <QApplication>


int main( int argc, char **argv )
{
    QFont font("ZYSong18030",12);
    QApplication::setFont(font);
    	
    QApplication a( argc, argv );
    QTranslator translator(0);
    translator.load("fileinformation_zh",".");
    a.installTranslator(&translator);    
    
    FileInformation *fileinformation = new FileInformation();
    fileinformation->show();
    return a.exec();
}

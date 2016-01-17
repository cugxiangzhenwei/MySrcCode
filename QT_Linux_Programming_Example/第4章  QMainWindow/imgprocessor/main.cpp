#include "imgprocessor.h"
#include <QApplication>


int main( int argc, char **argv )
{
    QFont font("ZYSong18030",10);
    QApplication::setFont(font);	
    
    QApplication a( argc, argv );
    QTranslator translator(0);
    translator.load("imgprocessor_zh",".");
    a.installTranslator(&translator);
    
    ImgProcessor *imgprocessor = new ImgProcessor(); 
    imgprocessor->show();
    return a.exec();
}

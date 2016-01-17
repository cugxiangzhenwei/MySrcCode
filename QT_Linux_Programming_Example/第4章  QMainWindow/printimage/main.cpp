#include "printimage.h"
#include <QApplication>


int main( int argc, char **argv )
{
    QFont font("ZYSong18030",10);
    QApplication::setFont(font);	
    
    QApplication a( argc, argv );
    QTranslator translator(0);
    translator.load("printimage_zh",".");
    a.installTranslator(&translator);
    
    PrintImage *printimage = new PrintImage(); 
    printimage->show();
    return a.exec();
}

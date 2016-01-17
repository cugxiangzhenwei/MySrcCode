#include "printtext.h"
#include <QApplication>


int main( int argc, char **argv )
{
    QFont font("ZYSong18030",12);
    QApplication::setFont(font);	
    
    QApplication a( argc, argv );
    QTranslator translator(0);
    translator.load("printtext_zh",".");
    a.installTranslator(&translator);
    
    PrintText *printtext = new PrintText(); 
    printtext->show();
    return a.exec();
}

#include "stringcodec.h"
#include <QApplication>

int main( int argc, char **argv )
{
    QApplication a( argc, argv );
    QTranslator translator(0);
    translator.load("stringcodec_zh",".");
    a.installTranslator(&translator);  
        
    StringCodec *stringcodec = new StringCodec();
    stringcodec->show();
    return a.exec();
}

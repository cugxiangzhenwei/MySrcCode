#include "standarddialogs.h"
#include <QApplication>


int main( int argc, char **argv )
{
    QFont font("ZYSong18030",12);
    QApplication::setFont(font);
    	
    QApplication a( argc, argv );
    QTranslator translator(0);
    translator.load("standarddialogs_zh",".");
    a.installTranslator(&translator);    

    StandardDialogs *standarddialogs = new StandardDialogs();
    standarddialogs->show();
    return a.exec();
}

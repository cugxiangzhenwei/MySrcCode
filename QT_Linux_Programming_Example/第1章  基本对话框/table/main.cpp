#include "table.h"
#include <QApplication>


int main( int argc, char **argv )
{
    QFont font("ZYSong18030",12);
    QApplication::setFont(font);
    	
    QApplication a( argc, argv );
    QTranslator translator(0);
    translator.load("table_zh",".");
    a.installTranslator(&translator);    
    
    Table *table = new Table();
    table->show();
    return a.exec();
}

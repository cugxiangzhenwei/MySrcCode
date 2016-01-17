#include "eventfilter.h"
#include <QApplication>


int main( int argc, char **argv )
{   	
    QApplication a( argc, argv );
    QTranslator translator(0);
    translator.load("eventfilter_zh",".");
    a.installTranslator(&translator);    
    
    EventFilter *eventfilter = new EventFilter();
    eventfilter->show();
    return a.exec();
}

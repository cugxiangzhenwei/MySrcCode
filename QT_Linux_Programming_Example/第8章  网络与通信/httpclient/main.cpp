#include "httpclient.h"
#include <QApplication>


int main( int argc, char **argv )
{   	
    QApplication a( argc, argv );
    QTranslator translator(0);
    translator.load("httpclient_zh",".");
    a.installTranslator(&translator);    
    
    HttpClient *httpclient = new HttpClient();
    httpclient->show();
    return a.exec();
}

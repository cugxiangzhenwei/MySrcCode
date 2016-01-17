#include "udpclient.h"
#include <QApplication>


int main( int argc, char **argv )
{
    QFont font("ZYSong18030",12);
    QApplication::setFont(font);
    	
    QApplication a( argc, argv );
    QTranslator translator(0);
    translator.load("udpclient_zh",".");
    a.installTranslator(&translator);    
    
    UdpClient *udpclient = new UdpClient();
    udpclient->show();
    return a.exec();
}

#include "udpserver.h"
#include <QApplication>


int main( int argc, char **argv )
{
    QFont font("ZYSong18030",12);
    QApplication::setFont(font);
    	
    QApplication a( argc, argv );
    QTranslator translator(0);
    translator.load("udpserver_zh",".");
    a.installTranslator(&translator);    
    
    UdpServer *udpserver = new UdpServer();
    udpserver->show();
    return a.exec();
}

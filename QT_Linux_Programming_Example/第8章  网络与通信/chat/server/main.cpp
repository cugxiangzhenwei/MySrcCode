#include "tcpserver.h"
#include <QApplication>


int main( int argc, char **argv )
{
   	
    QApplication a( argc, argv );
    QTranslator translator(0);
    translator.load("tcpserver_zh",".");
    a.installTranslator(&translator);    
    
    TcpServer *tcpserver = new TcpServer();
    tcpserver->show();
    return a.exec();
}

#include "tcpclient.h"
#include <QApplication>


int main( int argc, char **argv )
{
    	
    QApplication a( argc, argv );
    QTranslator translator(0);
    translator.load("tcpclient_zh",".");
    a.installTranslator(&translator);    
    
    TcpClient *tcpclient = new TcpClient();
    tcpclient->show();
    return a.exec();
}

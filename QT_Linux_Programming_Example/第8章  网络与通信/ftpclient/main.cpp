#include "ftpclient.h"
#include <QApplication>


int main( int argc, char **argv )
{
    QFont font("ZYSong18030",12);
    QApplication::setFont(font);
    	
    QApplication a( argc, argv );
    QTranslator translator(0);
    translator.load("ftpclient_zh",".");
    a.installTranslator(&translator);    
    
    FtpClient *ftpclient = new FtpClient();
    ftpclient->show();
    return a.exec();
}

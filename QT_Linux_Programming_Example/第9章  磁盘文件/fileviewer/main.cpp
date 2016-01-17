#include "fileviewer.h"
#include <QApplication>


int main( int argc, char **argv )
{
    QFont font("ZYSong18030",12);
    QApplication::setFont(font);
    	
    QApplication a( argc, argv );
    QTranslator translator(0);
    translator.load("fileviewer_zh",".");
    a.installTranslator(&translator);    
    
    FileViewer *fileviewer = new FileViewer();
    fileviewer->show();
    return a.exec();
}

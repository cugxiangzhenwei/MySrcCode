#include <QApplication>
#include "mainwindow.h"

int main(int argc, char * argv[])
{
    QApplication app(argc,argv);
    QFont font("ZYSong18030",12);
    app.setFont(font); 
       
    QTranslator translator;
    translator.load("workspace_zh");
    app.installTranslator(&translator);
    
    MainWidget w;
    w.show();

    return app.exec();
}

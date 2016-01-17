#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    QFont font("ZYSong18030",12);
    app.setFont(font);
    
    QTranslator translator;
    translator.load("view_zh");
    app.installTranslator(&translator);
    
    MainWindow win;
    win.show();
    
    return app.exec();
}


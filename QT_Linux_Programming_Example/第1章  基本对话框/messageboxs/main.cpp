#include <QApplication>
#include <QTranslator>
#include "messagebox.h"

int  main(int argc, char * argv[])
{
    QApplication app(argc,argv);
    QFont f("ZYSong18030",12);    QApplication::setFont(f);
    
    QTranslator translator;
    translator.load("messagebox_zh");
    app.installTranslator(&translator);
    
    MessageBox msg;
    msg.show();
    return app.exec();
}

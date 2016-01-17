#include <QApplication>
#include <QTranslator>

#include "pararw.h"

int main(int argc, char *argv[])
{
 //   QFont f("ZYSong18030",12);
 //   QApplication::setFont(f);
    QApplication app(argc,argv);
    
    QTranslator translator;
    translator.load("datastream_zh");
    app.installTranslator(&translator);
    
    ParaRW *para = new ParaRW();
    para->show();
    return app.exec();
}

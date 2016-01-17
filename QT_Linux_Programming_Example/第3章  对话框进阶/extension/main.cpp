#include <QApplication>
#include "extension.h"

int main(int argc, char * argv[])
{
    QApplication app(argc,argv);
    QFont f("ZYSong18030",12);
    QApplication::setFont(f);
    
    QTranslator translator;
    translator.load("extension_zh");
    app.installTranslator(&translator);
    
    Extension ex;
    ex.show();
    return app.exec();
}

#include <QApplication>
#include "listalign.h"

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);
    
    QTranslator translator;
    translator.load("listalign_zh");
    app.installTranslator(&translator);
    ListAlign *t = new ListAlign();
    t->show();
    return app.exec();
}

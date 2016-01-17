#include <QApplication>
#include <QTranslator>

#include "toolbar.h"

int main(int argc, char *argv[])
{
//	QFont f("ZYSong18030",10);
//    QApplication::setFont(f);
    QApplication app(argc,argv);
    
    QTranslator translator;
    translator.load("toolbar_zh");
    app.installTranslator(&translator);
    ToolBar *t = new ToolBar();
    t->show();
    return app.exec();
}

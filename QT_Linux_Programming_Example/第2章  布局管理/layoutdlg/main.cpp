#include <QApplication>
#include "layoutdlg.h"

int main( int argc, char * argv[] )
{
    QFont font("ZYSong18030",12);
    QApplication::setFont(font);
    		
    QApplication app(argc, argv);
    QTranslator translator(0);
    translator.load("layoutdlg_zh",".");
    app.installTranslator(&translator);    
    
    LayoutDlg layoutDlg;
    layoutDlg.show();
    return app.exec();
}

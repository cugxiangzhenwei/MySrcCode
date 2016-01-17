#include <QApplication>
#include "stack.h"

int main( int argc, char * argv[] )
{
    QFont font("ZYSong18030",12);
    QApplication::setFont(font);
    	
    QApplication a( argc, argv );
    QTranslator translator(0);
    translator.load("stack_zh",".");
    a.installTranslator(&translator); 
    
    StackDlg stack;
    stack.show();
    return a.exec();
}

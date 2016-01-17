#include "inputdialog.h"
#include <QApplication>

int main( int argc, char **argv )
{
    QFont font("ZYSong18030",12);
    QApplication::setFont(font);
    	
    QApplication app( argc, argv );
    QTranslator translator;
    translator.load("inputdialog_zh");
    app.installTranslator(&translator);
    InputDlg *input = new InputDlg();
    input->show();
    return app.exec();
}

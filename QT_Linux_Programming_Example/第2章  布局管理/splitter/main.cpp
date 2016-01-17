#include <QApplication>
#include <QtGui>

int main( int argc, char * argv[] )
{
    QFont font("ZYSong18030",12);
    QApplication::setFont(font);
    	
    QApplication app(argc, argv);
    QTranslator translator(0);
    translator.load("splitter_zh",".");
    app.installTranslator(&translator); 
        
    QSplitter *splitterMain = new QSplitter(Qt::Horizontal,0);
    QTextEdit *textLeft = new QTextEdit( QObject::tr("Left Widget"),splitterMain);
    textLeft->setAlignment(Qt::AlignCenter);
    
    QSplitter *splitterRight = new QSplitter(Qt::Vertical,splitterMain);  
    splitterRight->setOpaqueResize(false);
    QTextEdit *textUp = new QTextEdit( QObject::tr("Top Widget"),splitterRight);
    textUp->setAlignment(Qt::AlignCenter);
    QTextEdit *textBottom = new QTextEdit( QObject::tr("Bottom Widget"),splitterRight);
    textBottom->setAlignment(Qt::AlignCenter);
    
    splitterMain->setStretchFactor(1,1);
    splitterMain->setWindowTitle( QObject::tr("Splitter"));

    splitterMain->show();
    return app.exec();
}

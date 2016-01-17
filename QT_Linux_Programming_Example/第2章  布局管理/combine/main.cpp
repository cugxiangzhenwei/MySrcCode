#include <QApplication>
#include <QTranslator>
#include <QTextEdit>
#include <QListWidget>
#include <QSplitter>
#include <QTextCodec>
#include <QStackedWidget>

#include "content.h"

int main( int argc, char * argv[] )
{
    QApplication app(argc, argv);
    QFont font("AR PL KaitiM GB",12);
    app.setFont(font);
    
    QTranslator translator;
    translator.load("combine_zh");
    app.installTranslator(&translator);    
 
    QSplitter *splitterMain = new QSplitter(Qt::Horizontal,0);
    splitterMain->setOpaqueResize(true);
    QListWidget *list = new QListWidget(splitterMain);
    list->insertItem(0,QObject::tr("Base Info"));
    list->insertItem(1,QObject::tr("Contact"));
    list->insertItem(2,QObject::tr("Detail"));
    
    Content *content = new Content(splitterMain);
    
    QObject::connect(list,SIGNAL(currentRowChanged(int)),content->stack,SLOT(setCurrentIndex(int)));
    
    splitterMain->setWindowTitle(QObject::tr("Change User Info"));
    splitterMain->setMinimumSize(splitterMain->minimumSize());
    splitterMain->setMaximumSize(splitterMain->minimumSize());
    splitterMain->show();
    return app.exec();
}

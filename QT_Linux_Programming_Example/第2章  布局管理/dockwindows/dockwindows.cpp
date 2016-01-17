#include "dockwindows.h"

DockWindows::DockWindows()
    : QMainWindow()
{
    setWindowTitle( tr( "DockWindows" ) );
    
    QTextEdit *te = new QTextEdit( this );
    te->setText(tr("Main Window"));   
    te->setAlignment(Qt::AlignCenter);
    setCentralWidget( te ); 

    //  Í£¿¿´°¿Ú 1
    QDockWidget *dock = new QDockWidget(tr("DockWindow 1"), this );
    dock->setFeatures( QDockWidget::DockWidgetMovable );
    dock->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
    QTextEdit *te1 = new QTextEdit();
    te1->setText(tr("Window 1"));   
    dock->setWidget( te1 );
    addDockWidget( Qt::RightDockWidgetArea, dock );
    
    //  Í£¿¿´°¿Ú 2
    dock = new QDockWidget( tr("DockWindow 2"), this );
    dock->setFeatures( QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetClosable);
    QTextEdit *te2 = new QTextEdit();
    te2->setText(tr("Window 2"));   
    dock->setWidget( te2 );   
    addDockWidget( Qt::RightDockWidgetArea, dock);   

    //  Í£¿¿´°¿Ú 3
    dock = new QDockWidget( tr("DockWindow 3"),this );
    dock->setFeatures( QDockWidget::AllDockWidgetFeatures );
    QTextEdit *te3 = new QTextEdit();
    te3->setText(tr("Window 3"));   
    dock->setWidget( te3 );     
    addDockWidget( Qt::BottomDockWidgetArea, dock); 
}

DockWindows::~DockWindows()
{
}


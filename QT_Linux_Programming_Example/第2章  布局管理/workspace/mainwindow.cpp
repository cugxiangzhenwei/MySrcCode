#include "mainwindow.h"

MainWidget::MainWidget(QWidget *parent)
	: QMainWindow(parent)
{   
    setWindowTitle(tr("Workspace"));
    
    workSpace = new QWorkspace;
    setCentralWidget(workSpace);  
    
    createMenu(); 
    
    QMainWindow *window1 = new QMainWindow;
    window1->setWindowTitle(tr("window 1"));
    QTextEdit *edit1 = new QTextEdit;
    edit1->setText(tr("Window 1"));
    window1->setCentralWidget(edit1);
    
    QMainWindow *window2 = new QMainWindow;
    window2->setWindowTitle(tr("window 2"));
    QTextEdit *edit2 = new QTextEdit;
    edit2->setText(tr("Window 2"));
    window2->setCentralWidget(edit2);
    
    QMainWindow *window3 = new QMainWindow;
    window3->setWindowTitle(tr("window 3"));
    QTextEdit *edit3 = new QTextEdit;
    edit3->setText(tr("Window 3"));
    window3->setCentralWidget(edit3);
    
    workSpace->addWindow(window1);
    workSpace->addWindow(window2);
    workSpace->addWindow(window3);

}

void MainWidget::createMenu()
{
    QMenu *layoutMenu = menuBar()->addMenu(tr("Layout"));
    
    QAction *arrange = new QAction(tr("Arrange Icons"),this);
    connect(arrange,SIGNAL(triggered()),workSpace,SLOT(arrangeIcons()));
    layoutMenu->addAction(arrange);
    
    QAction *tile = new QAction(tr("tile"),this);
    connect(tile,SIGNAL(triggered()),workSpace,SLOT(tile()));
    layoutMenu->addAction(tile);
    
    QAction *cascade = new QAction(tr("cascade"),this);
    connect(cascade,SIGNAL(triggered()),workSpace,SLOT(cascade()));
    layoutMenu->addAction(cascade);
    
    
    QMenu *otherMenu = menuBar()->addMenu(tr("Other"));
    
    QAction *scrollAct = new QAction(tr("Scroll"),this);
    scrollAct->setCheckable(true);
    connect(scrollAct,SIGNAL(toggled(bool)),this,SLOT(slotScroll(bool)));
    otherMenu->addAction(scrollAct);
    otherMenu->addSeparator();

    
    QAction *nextAct = new QAction(tr("Next"),this);
    connect(nextAct,SIGNAL(triggered()),workSpace,SLOT(activateNextWindow()));
    otherMenu->addAction(nextAct);
    
    QAction *previousAct = new QAction(tr("Previous"),this);
    connect(previousAct,SIGNAL(triggered()),workSpace,SLOT(activatePreviousWindow()));
    otherMenu->addAction(previousAct);
}

void MainWidget::slotScroll(bool check)
{
	if(check)
	{
    	workSpace->setScrollBarsEnabled(true);    
    }
    else
    {
    	workSpace->setScrollBarsEnabled(false);    
    }

}




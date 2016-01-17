#include "printimage.h"

PrintImage::PrintImage()
    : QMainWindow()
{  
    QFont f("ZYSong18030",12);
    setFont(f);
    		  
    setWindowTitle(tr("PrintImage"));
    
    imageLabel = new QLabel(this);  
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);
	setCentralWidget(imageLabel); 
	
    createActions();
    createMenus();
    createToolBars();
      
    if(image.load("tux.png"));
    {
    		imageLabel->setPixmap (QPixmap::fromImage(image));
    		resize(image.width(), image.height());
    }
}

PrintImage::~PrintImage()
{
}

void PrintImage::createActions()
{
	
    // print action
    printAction = new QAction(QIcon(":/images/print.png"),tr("Print"),this);
    printAction->setShortcut(tr("Ctrl+P"));
    printAction->setStatusTip(tr("Print"));
    connect(printAction,SIGNAL(triggered()),this,SLOT(slotPrint()));
}

void PrintImage::createMenus()
{
    printMenu = menuBar()->addMenu(tr("Print"));
    printMenu->addAction(printAction);
}


void PrintImage::createToolBars()
{
    printTool = addToolBar("Print");
    printTool->addAction(printAction);
}

void PrintImage::slotPrint ()
{
	QPrinter printer;   	
    QPrintDialog printDialog(&printer, this);
    if (printDialog.exec()) 
    {
        QPainter painter(&printer);
        QRect rect = painter.viewport();
        qWarning("rect.x()=%d,rect.y()=%d",rect.x(),rect.y());
        qWarning("rect.width()=%d,rect.height()=%d",rect.width(),rect.height());
        QSize size = image.size();
        qWarning("size.width()=%d,size.height()=%d",size.width(),size.height());
        size.scale(rect.size(), Qt::KeepAspectRatio);
        qWarning("size.width()=%d,size.height()=%d",size.width(),size.height());
        painter.setViewport(rect.x(), rect.y(),size.width(), size.height());
        painter.setWindow(image.rect());
        painter.drawImage(0, 0, image);
    }
}

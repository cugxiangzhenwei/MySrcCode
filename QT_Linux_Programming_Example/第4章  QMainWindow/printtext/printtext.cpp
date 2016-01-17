#include "printtext.h"

PrintText::PrintText()
    : QMainWindow()
{  
    QFont f("ZYSong18030",12);
    setFont(f);
    		  
    setWindowTitle(tr("Printer"));
    
    contentTextEdit = new QTextEdit(this);
	setCentralWidget(contentTextEdit); 
	
    createActions();
    createMenus();
    createToolBars();
      
     
    QFile file("QPrinter.txt");
    if (file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
    	QTextStream textStream(&file);
    	while(!textStream.atEnd())
    	{
    	    contentTextEdit->append(textStream.readLine());
    	}
    	file.close();
    }
}

PrintText::~PrintText()
{
}

void PrintText::createActions()
{
    // print action
    printAction = new QAction(QIcon(":/images/print.png"),tr("Print"),this);
    printAction->setShortcut(tr("Ctrl+P"));
    printAction->setStatusTip(tr("Print"));
    connect(printAction,SIGNAL(triggered()),this,SLOT(slotPrint()));
}

void PrintText::createMenus()
{
    printMenu = menuBar()->addMenu(tr("Print"));
    printMenu->addAction(printAction);
}


void PrintText::createToolBars()
{
    printTool = addToolBar("Print");
    printTool->addAction(printAction);
}

void PrintText::slotPrint ()
{
	QPrinter printer;   	
    QPrintDialog printDialog(&printer, this);
    if (printDialog.exec()) 
    {	
		QTextDocument *doc = contentTextEdit->document();
		doc->print(&printer);
	}
}

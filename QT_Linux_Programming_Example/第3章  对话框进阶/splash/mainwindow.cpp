#include "mainwindow.h"  

#include <QtGui>
#include <QtTest>
//#include <unistd.h>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
    setWindowTitle(tr("Splash Example"));

    QTextEdit *edit = new QTextEdit;
    edit->setText("Splash Example !");
    setCentralWidget(edit);
   
    createToolBars();
    createStatusBar();
    
    resize(600,450);
    
//    sleep(3);
    QTest::qWait(3000);
}

void
MainWindow::createToolBars()
{
    QToolBar *fileTool = addToolBar("File");
    QToolBar *editTool = addToolBar("Edit");
    
    QToolButton *fileNewBtn = new QToolButton;
    fileNewBtn->setIcon(QIcon(":/images/new.png"));
    QToolButton *fileOpenBtn = new QToolButton;
    fileOpenBtn->setIcon(QIcon(":/images/open.png"));
    QToolButton *fileSaveBtn = new QToolButton;
    fileSaveBtn->setIcon(QIcon(":/images/save.png"));
    
    fileTool->addWidget(fileNewBtn);
    fileTool->addWidget(fileOpenBtn);
    fileTool->addWidget(fileSaveBtn);
    
    QToolButton *copyBtn = new QToolButton;
    copyBtn->setIcon(QIcon(":/images/copy.png"));
    QToolButton *cutBtn = new QToolButton;
    cutBtn->setIcon(QIcon(":/images/cut.png"));
    QToolButton *pasteBtn = new QToolButton;
    pasteBtn->setIcon(QIcon(":/images/paste.png"));
    
    editTool->addWidget(copyBtn);
    editTool->addWidget(cutBtn);
    editTool->addWidget(pasteBtn);
}

void
MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}


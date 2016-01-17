#include "mainwindow.h"  

#include <QtGui>

// 主窗口实现
MainWindow::MainWindow()
{
    setWindowTitle(tr("Doc"));
    text = new QTextEdit(this);
    setCentralWidget(text);
   
    createActions();
    createMenus();
    createToolBars();
}

void
MainWindow::createActions()
{
	
    // file open action
    fileOpenAction = new QAction(QIcon(":/images/open.png"),tr("Open"),this);	// 打开文件
    fileOpenAction->setShortcut(tr("Ctrl+O"));
    fileOpenAction->setStatusTip(tr("open a file"));
    connect(fileOpenAction,SIGNAL(triggered()),this,SLOT(slotOpenFile()));

    
    // file new action
    fileNewAction = new QAction(QIcon(":/images/new.png"),tr("New"),this);	// 新建文件
    fileNewAction->setShortcut(tr("Ctrl+N"));
    fileNewAction->setStatusTip(tr("new file"));
    connect(fileNewAction,SIGNAL(triggered()),this,SLOT(slotNewFile()));

    // save file action
    fileSaveAction = new QAction(QPixmap(":/images/save.png"),tr("Save"),this);	// 保存文件 
    fileSaveAction->setShortcut(tr("Ctrl+S"));
    fileSaveAction->setStatusTip(tr("save file"));
    connect(fileSaveAction,SIGNAL(activated()),this,SLOT(slotSaveFile()));

    // exit action
    exitAction = new QAction(tr("Exit"), this);	// 退出
    exitAction->setShortcut(tr("Ctrl+Q"));
    exitAction->setStatusTip(tr("exit"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    cutAction = new QAction(QIcon(":/images/cut.png"), tr("Cut"), this);		// 剪切
    cutAction->setShortcut(tr("Ctrl+X"));
    cutAction->setStatusTip(tr("cut to clipboard"));
    connect(cutAction, SIGNAL(triggered()), text, SLOT(cut()));

    copyAction = new QAction(QIcon(":/images/copy.png"), tr("Copy"), this);		// 复制
    copyAction->setShortcut(tr("Ctrl+C"));
    copyAction->setStatusTip(tr("copy to clipboard"));
    connect(copyAction, SIGNAL(triggered()), text, SLOT(copy()));

    pasteAction = new QAction(QIcon(":/images/paste.png"), tr("Paste"), this);		// 粘贴
    pasteAction->setShortcut(tr("Ctrl+V"));
    pasteAction->setStatusTip(tr("paste clipboard to selection"));
    connect(pasteAction, SIGNAL(triggered()), text, SLOT(paste()));

    aboutAction = new QAction(tr("About"), this);		// 关于
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(slotAbout()));
}

void
MainWindow::createMenus()
{
	
    fileMenu = menuBar()->addMenu(tr("File"));
    editMenu = menuBar()->addMenu(tr("Edit"));
    aboutMenu = menuBar()->addMenu(tr("Help"));
    
    fileMenu->addAction(fileNewAction);
    fileMenu->addAction(fileOpenAction);
    fileMenu->addAction(fileSaveAction);
    fileMenu->addAction(exitAction);
    
    editMenu->addAction(copyAction);
    editMenu->addAction(cutAction);
    editMenu->addAction(pasteAction);
    
    aboutMenu->addAction(aboutAction);
         
}


void
MainWindow::createToolBars()
{
	
    fileTool = addToolBar("File");
    fileTool->setMovable(false);
    editTool = addToolBar("Edit");
    
    fileTool->addAction(fileNewAction);
    fileTool->addAction(fileOpenAction);
    fileTool->addAction(fileSaveAction);
    
    editTool->addAction(copyAction);
    editTool->addAction(cutAction);
    editTool->addAction(pasteAction);
   
}

void
MainWindow::slotNewFile()
{
}


void
MainWindow::slotOpenFile()
{
}

void
MainWindow::slotSaveFile()
{
}

void
MainWindow::slotCopy()
{
}

void
MainWindow::slotCut()
{
}

void
MainWindow::slotPaste()
{
}

void
MainWindow::slotAbout()
{
}



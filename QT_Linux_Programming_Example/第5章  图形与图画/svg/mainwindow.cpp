#include "mainwindow.h"

MainWidget::MainWidget(QWidget *parent)
	: QMainWindow(parent)
{   
    setWindowTitle(tr("SVG Viewer"));
   
    createMenu(); 

    svgWindow = new SvgWindow;
    setCentralWidget(svgWindow);
}

void MainWidget::createMenu()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("File"));
    
    QAction *openAct = new QAction(tr("Open"),this);
    connect(openAct,SIGNAL(triggered()),this,SLOT(slotOpenFile()));
    fileMenu->addAction(openAct);
}

void MainWidget::slotOpenFile()
{
    QString name = QFileDialog::getOpenFileName(
        this, 
        "open file dialog",
        "/",
        "svg files (*.svg)"); 
    
    svgWindow->setFile(name);
}






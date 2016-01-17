#include "imgprocessor.h"

ImgProcessor::ImgProcessor()
    : QMainWindow()
{  
    QFont f("ZYSong18030",12);
    setFont(f);
    		  
    setWindowTitle(tr("Image Processor"));
    

    imageLabel = new QLabel(this);
    
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);
   
    setCentralWidget(imageLabel);
    
    createActions();
    createMenus();
    createToolBars();

}

ImgProcessor::~ImgProcessor()
{
}

void ImgProcessor::createActions()
{
	
    // open file action
    openFileAction = new QAction(QIcon(":/images/openfile.png"),tr("Open"),this);
    openFileAction->setShortcut(tr("Ctrl+O"));
    openFileAction->setStatusTip(tr("Open File"));
    connect(openFileAction,SIGNAL(triggered()),this,SLOT(slotOpenFile()));

    // zoom in action
    zoomInAction = new QAction(QIcon(":/images/zoomin.png"),tr("ZoomIn"),this);
    zoomInAction->setShortcut(tr("PgUp"));
    zoomInAction->setStatusTip(tr("Zoom In"));
    connect(zoomInAction,SIGNAL(triggered()),this,SLOT(slotZoomIn()));

    // zoom out action
    zoomOutAction = new QAction(QIcon(":/images/zoomout.png"),tr("ZoomOut"),this);
    zoomOutAction->setShortcut(tr("PgDown"));
    zoomOutAction->setStatusTip(tr("Zoom Out"));
    connect(zoomOutAction,SIGNAL(triggered()),this,SLOT(slotZoomOut()));

    // rotate 90 action
    rotate90Action = new QAction(QIcon(":/images/rotate90.png"),tr("Rotate90"), this);
    rotate90Action->setShortcut(tr("Ctrl+Right"));
    rotate90Action->setStatusTip(tr("Rotate 90"));
    connect(rotate90Action, SIGNAL(triggered()), this, SLOT(slotRotate90()));

    // rotate 180 action
    rotate180Action = new QAction(QIcon(":/images/rotate180.png"),tr("Rotate180"), this);
    rotate180Action->setShortcut(tr("Ctrl+Down"));
    rotate180Action->setStatusTip(tr("Rotate 180"));
    connect(rotate180Action, SIGNAL(triggered()), this, SLOT(slotRotate180()));
    
    // rotate 270 action
    rotate270Action = new QAction(QIcon(":/images/rotate270.png"),tr("Rotate270"), this);
    rotate270Action->setShortcut(tr("Ctrl+Left"));
    rotate270Action->setStatusTip(tr("Rotate 270"));
    connect(rotate270Action, SIGNAL(triggered()), this, SLOT(slotRotate270()));    

    // mirror vertical action
    mirrorVerticalAction = new QAction(QIcon(":/images/mirrorvertical.png"),tr("MirrorVertical"), this);
    mirrorVerticalAction->setShortcut(tr("Ctrl+V"));
    mirrorVerticalAction->setStatusTip(tr("Mirror Vertical"));
    connect(mirrorVerticalAction, SIGNAL(triggered()), this, SLOT(slotMirrorVertical())); 

    // mirror horizontal action
    mirrorHorizontalAction = new QAction(QIcon(":/images/mirrorhorizontal.png"),tr("MirrorHorizontal"), this);
    mirrorHorizontalAction->setShortcut(tr("Ctrl+H"));
    mirrorHorizontalAction->setStatusTip(tr("Mirror Horizontal"));
    connect(mirrorHorizontalAction, SIGNAL(triggered()), this, SLOT(slotMirrorHorizontal())); 
}

void ImgProcessor::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("File"));
    zoomMenu = menuBar()->addMenu(tr("Zoom"));
    rotateMenu = menuBar()->addMenu(tr("rotate"));
    mirrorMenu = menuBar()->addMenu(tr("mirror"));
    
    fileMenu->addAction(openFileAction);

    zoomMenu->addAction(zoomInAction);
    zoomMenu->addAction(zoomOutAction);

    rotateMenu->addAction(rotate90Action);
    rotateMenu->addAction(rotate180Action);
    rotateMenu->addAction(rotate270Action);

    mirrorMenu->addAction(mirrorVerticalAction);
    mirrorMenu->addAction(mirrorHorizontalAction);

}


void ImgProcessor::createToolBars()
{
    fileTool = addToolBar("File");
    zoomTool = addToolBar("Zoom");
    rotateTool = addToolBar("Rotate");
    mirrorTool = addToolBar("Mirror");
    
    fileTool->addAction(openFileAction);
    
    zoomTool->addAction(zoomInAction);
    zoomTool->addAction(zoomOutAction);
    
    rotateTool->addAction(rotate90Action);   
    rotateTool->addAction(rotate180Action);   
    rotateTool->addAction(rotate270Action);   
    
    mirrorTool->addAction(mirrorVerticalAction);   
    mirrorTool->addAction(mirrorHorizontalAction);   
}

void ImgProcessor::slotOpenFile ()
{
    QString s = QFileDialog::getOpenFileName(               
	this, "open image file",
        ".",
        "Image files (*.bmp *.jpg *.pbm *.pgm *.png *.ppm *.xbm *.xpm);;All files (*.*)");                
    if(s != "")
    {
    	if(img.load(s));
    	{
    		imageLabel->setPixmap (QPixmap::fromImage(img));
    		resize(img.width(), img.height());
    	}
    }
}

void ImgProcessor::slotZoomIn ()
{
	if(img.isNull())
	{
		return;
	}

	QMatrix martix;
	martix.scale (2,2);  
	img=img.transformed(martix);
	imageLabel->setPixmap (QPixmap::fromImage(img));
	resize(img.width(),img.height());  	
}

void ImgProcessor::slotZoomOut ()
{
	if(img.isNull())
	{
		return;
	}

	QMatrix martix;
	martix.scale(0.5,0.5);  
	img=img.transformed(martix);
	imageLabel->setPixmap (QPixmap::fromImage(img));
	resize(img.width(),img.height());
}

void ImgProcessor::slotRotate90 ()
{
	if(img.isNull())
	{
		return;
	}
	QMatrix martix;
	martix.rotate(90);  
	img=img.transformed(martix);
	imageLabel->setPixmap (QPixmap::fromImage(img));
	resize(img.width(),img.height());

}

void ImgProcessor::slotRotate180 ()
{
	if(img.isNull())
	{
		return;
	}
	QMatrix martix;
	martix.rotate(180);  
	img=img.transformed(martix);
	imageLabel->setPixmap (QPixmap::fromImage(img));
	resize(img.width(),img.height());
}

void ImgProcessor::slotRotate270 ()
{
	if(img.isNull())
	{
		return;
	}
	QMatrix martix;
	martix.rotate(270);  
	img=img.transformed(martix);
	imageLabel->setPixmap (QPixmap::fromImage(img));
	resize(img.width(),img.height()); 
}

void ImgProcessor::slotMirrorVertical ()
{
	if(img.isNull())
	{
		return;
	}

	img=img.mirrored(false,true);
	imageLabel->setPixmap (QPixmap::fromImage(img));
	resize(img.width(),img.height()); 
}

void ImgProcessor::slotMirrorHorizontal ()
{
	if(img.isNull())
	{
		return;
	}
 
	img=img.mirrored(true,false);
	imageLabel->setPixmap (QPixmap::fromImage(img));
	resize(img.width(),img.height()); 
}

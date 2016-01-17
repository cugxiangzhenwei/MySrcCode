#include "mainwidget.h"
#include <QtGui>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
    createActions();
    createMenus();
    
    scene = new QGraphicsScene;
    scene->setSceneRect(-200,-200,400,400);
    
    initScene();
    
    QGraphicsView *view = new QGraphicsView;
    view->setScene(scene);
    view->setMinimumSize(400,400);
    view->show();
    
    setCentralWidget(view);
    resize(550,450);
    setWindowTitle(tr("Graphics Items"));
}

void
MainWindow::createActions()
{
    newAct = new QAction(tr("New"),this);
    newAct->setStatusTip(tr("New"));
    connect(newAct,SIGNAL(triggered()),this,SLOT(slotNew()));
   
    clearAct = new QAction(tr("Clear"),this);
    clearAct->setStatusTip(tr("Clear"));
    connect(clearAct,SIGNAL(triggered()),this,SLOT(slotClear()));
   
    exitAct = new QAction(tr("Exit"),this);
    connect(exitAct,SIGNAL(triggered()),this,SLOT(close()));
    
    addEllipseItemAct = new QAction(tr("Add Ellipse"),this);
    connect(addEllipseItemAct,SIGNAL(triggered()),this,SLOT(slotAddEllipseItem()));
   
    addPolygonItemAct = new QAction(tr("Add Polygon"),this);
    connect(addPolygonItemAct,SIGNAL(triggered()),this,SLOT(slotAddPolygonItem()));
    
    addTextItemAct = new QAction(tr("Add Text"),this);
    connect(addTextItemAct,SIGNAL(triggered()),this,SLOT(slotAddTextItem()));
     
    addFlashItemAct = new QAction(tr("Add Flash"),this);
    connect(addFlashItemAct,SIGNAL(triggered()),this,SLOT(slotAddFlashItem()));
    
    addRectItemAct = new QAction(tr("Add Rectangle"),this);
    connect(addRectItemAct,SIGNAL(triggered()),this,SLOT(slotAddRectItem()));
    
    addAnimItemAct = new QAction(tr("Add Animation"),this);
    connect(addAnimItemAct,SIGNAL(triggered()),this,SLOT(slotAddAnimationItem()));
    
    addAlphaItemAct = new QAction(tr("Add Alpha-png"),this);
    connect(addAlphaItemAct,SIGNAL(triggered()),this,SLOT(slotAddAlphaItem()));
}

void
MainWindow::createMenus()
{
    QMenu * fileMenu = menuBar()->addMenu(tr("File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(clearAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);
    
    QMenu * itemsMenu = menuBar()->addMenu(tr("Items"));
    itemsMenu->addAction(addEllipseItemAct);
    itemsMenu->addAction(addRectItemAct);
    itemsMenu->addAction(addPolygonItemAct);
    itemsMenu->addAction(addTextItemAct);
    itemsMenu->addAction(addFlashItemAct);
    itemsMenu->addAction(addAlphaItemAct);
    itemsMenu->addAction(addAnimItemAct);

}

void
MainWindow::initScene()
{
    int i;
    for (i=0; i<3; i++)
        slotAddFlashItem();
    for(i=0;i<3;i++)    
        slotAddEllipseItem();
    for(i=0;i<3;i++)    
        slotAddRectItem();
    for(i=0;i<2;i++)    
        slotAddAlphaItem();
    for(i=0;i<3;i++)    
        slotAddPolygonItem();
    for(i=0;i<3;i++)    
        slotAddTextItem();
}

void
MainWindow::slotNew()
{
    slotClear();
    initScene();
    MainWindow *newWin = new MainWindow;
    newWin->show();
}

void
MainWindow::slotClear()
{
    QList<QGraphicsItem*> listItem = scene->items();

    while (!listItem.empty())
    {
      	scene->removeItem(listItem.at(0));
      	listItem.removeAt(0);
    }
}


//   Add a ellipse item
void
MainWindow::slotAddEllipseItem()
{
    QGraphicsEllipseItem *item = new QGraphicsEllipseItem(QRectF(0,0,80,60));
    item->setPen(Qt::NoPen);
    item->setBrush(QColor(qrand()%256,qrand()%256,qrand()%256));
    qreal scale = ((qrand()%10)+1)/5.0;
    item->scale(scale,scale);
    item->setFlag(QGraphicsItem::ItemIsMovable);
    scene->addItem(item);
    item->setPos((qrand()%int(scene->sceneRect().width()))-200,(qrand()%int(scene->sceneRect().height()))-200);
}

//   Add a polygon item
void
MainWindow::slotAddPolygonItem()
{
    QVector<QPoint> v;
    v << QPoint(30,-15) << QPoint(0,-30) << QPoint(-30,-15) << QPoint(-30,15) << QPoint(0,30) << QPoint(30,15);
    QGraphicsPolygonItem *item = new QGraphicsPolygonItem(QPolygonF(v));
    item->setBrush(QColor(qrand()%256,qrand()%256,qrand()%256));
    item->setFlag(QGraphicsItem::ItemIsMovable);
    scene->addItem(item);
    item->setPos((qrand()%int(scene->sceneRect().width()))-200,(qrand()%int(scene->sceneRect().height()))-200);
}

//   Add a rectangle item
void
MainWindow::slotAddRectItem()
{
    QGraphicsRectItem *item = new QGraphicsRectItem(QRectF(0,0,60,60));
    QPen pen;
    pen.setWidth(3);
    pen.setColor(QColor(qrand()%256,qrand()%256,qrand()%256));
    item->setPen(pen);
    item->setBrush(QColor(qrand()%256,qrand()%256,qrand()%256));
    item->setFlag(QGraphicsItem::ItemIsMovable);
    scene->addItem(item);    
    item->setPos((qrand()%int(scene->sceneRect().width()))-200,(qrand()%int(scene->sceneRect().height()))-200);
}

//   Add a text item
void
MainWindow::slotAddTextItem()
{
    QFont font("Times",16);
    QGraphicsTextItem *item = new QGraphicsTextItem("Hello Qt");
    item->setFont(font);
    item->setFlag(QGraphicsItem::ItemIsMovable);
    item->setDefaultTextColor(QColor(qrand()%256,qrand()%256,qrand()%256));
    scene->addItem(item);
    item->setPos((qrand()%int(scene->sceneRect().width()))-200,(qrand()%int(scene->sceneRect().height()))-200);
}

//   Add a flash item
void
MainWindow::slotAddFlashItem()
{
    FlashItem *item = new FlashItem;
    qreal scale = ((qrand()%10)+1)/5.0;
    item->scale(scale,scale);
    scene->addItem(item);
    item->setPos((qrand()%int(scene->sceneRect().width()))-200,(qrand()%int(scene->sceneRect().height()))-200);
}

//   Add a butterfly item
void
MainWindow::slotAddAlphaItem()
{
    QGraphicsPixmapItem *item = scene->addPixmap(QPixmap(":/images/butterfly"));
    item->setFlag(QGraphicsItem::ItemIsMovable);
    item->setPos((qrand()%int(scene->sceneRect().width()))-200,(qrand()%int(scene->sceneRect().height()))-200);
    
}

void
MainWindow::slotAddAnimationItem()
{
    StarItem *item = new StarItem;
    QGraphicsItemAnimation *anim = new QGraphicsItemAnimation;
    anim->setItem(item);
    QTimeLine *timeLine = new QTimeLine(4000);
    timeLine->setCurveShape(QTimeLine::SineCurve);
    timeLine->setLoopCount(0);
    anim->setTimeLine(timeLine);
    
    int y = (qrand()%400) - 200;
    for (int i=0; i<400; i++)
    {
        anim->setPosAt(i/400.0, QPointF(i-200,y));
    }
    timeLine->start();
    scene->addItem(item);
}




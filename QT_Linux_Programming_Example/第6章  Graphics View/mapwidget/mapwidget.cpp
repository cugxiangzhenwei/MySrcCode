#include "mapwidget.h"
#include <QtGui>
#include <math.h>
MapWidget::MapWidget()
{ 
    readMap();   
//    map.load(":/images/map.png");
    zoom = 50;
    
    int width = map.width();
    int height = map.height();
    
    QGraphicsScene *scene = new QGraphicsScene(this);

    scene->setSceneRect(-width/2,-height/2,width,height);
    
    setScene(scene);
    setCacheMode(CacheBackground);
    
    QSlider *slider = new QSlider;
    slider->setOrientation(Qt::Vertical);
    slider->setRange(1,100);
    slider->setTickInterval(10);
    slider->setValue(50);
    connect(slider,SIGNAL(valueChanged(int)),this,SLOT(slotZoom(int)));
    
    QLabel *zoominLabel = new QLabel;
    zoominLabel->setScaledContents(true);
    zoominLabel->setPixmap(QPixmap(":/images/zoomin.png"));
    
    QLabel *zoomoutLabel = new QLabel;
    zoomoutLabel->setScaledContents(true);
    zoomoutLabel->setPixmap(QPixmap(":/images/zoomout.png"));
    
    //  create coordinate area
    QFrame *coordFrame = new QFrame;
    QLabel *label1 = new QLabel(tr("GraphicsView :"));
    viewCoord = new QLabel;
    QLabel *label2 = new QLabel(tr("GraphicsScene :"));
    sceneCoord = new QLabel;
    QLabel *label3 = new QLabel(tr("map :"));
    mapCoord = new QLabel;
    QGridLayout *grid = new QGridLayout;
    grid->addWidget(label1,0,0);
    grid->addWidget(viewCoord,0,1);
    grid->addWidget(label2,1,0);
    grid->addWidget(sceneCoord,1,1);
    grid->addWidget(label3,2,0);
    grid->addWidget(mapCoord,2,1);
    grid->setSizeConstraint(QLayout::SetFixedSize);
    coordFrame->setLayout(grid);
    
    //  zoom layout
    QVBoxLayout *zoomLayout = new QVBoxLayout;
    zoomLayout->addWidget(zoominLabel);
    zoomLayout->addWidget(slider);
    zoomLayout->addWidget(zoomoutLabel);
    
    //  coordinate area layou
    QVBoxLayout *coordLayout = new QVBoxLayout;
    coordLayout->addWidget(coordFrame);
    coordLayout->addStretch();
        
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addLayout(zoomLayout);
    layout->addLayout(coordLayout);
    layout->addStretch();
    layout->setMargin(30);
    layout->setSpacing(10);
    setLayout(layout);
    
    setWindowTitle("Map Widget");
    setMinimumSize(600,400);
}

// read map information
void
MapWidget::readMap()
{
    QFile mapFile("maps.txt"); 
    QString mapName;
    int ok = mapFile.open(QIODevice::ReadOnly);
    if (ok)
    {
    	QTextStream t(&mapFile);
    	if (!t.atEnd())
    	{
    	    t >> mapName;
    	    t >> x1 >> y1 >> x2 >> y2;
    	}
    }
  
    map.load(mapName);
    if (map.isNull())
    	printf("map is null");
}

void
MapWidget::slotZoom(int value)
{
    qreal s;
    if (value>zoom)	// zoom in
    {
  	s = pow(1.01,(value-zoom));
    }
    else		// zoom out
    {
	s = pow((1/1.01),(zoom-value));
    }

    scale(s,s);
    zoom = value;
}

void
MapWidget::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->drawPixmap(int(sceneRect().left()),int(sceneRect().top()),map);
}

void
MapWidget::mouseMoveEvent(QMouseEvent * event)
{
    // view coordinate
    QPoint viewPoint = event->pos();
    viewCoord->setText(QString::number(viewPoint.x()) + ", " + QString::number(viewPoint.y()));
    
    // scene coordinate
    QPointF scenePoint = mapToScene(viewPoint);
    sceneCoord->setText(QString::number(scenePoint.x()) + ", " + QString::number(scenePoint.y()));
    
    // map coordinate
    QPointF latLon = mapToMap(scenePoint);
    mapCoord->setText(QString::number(latLon.x()) + ", " + QString::number(latLon.y()));
}

// map scene coordinate to map
QPointF
MapWidget::mapToMap(QPointF p)
{
    QPointF latLon;
    qreal w = sceneRect().width();
    qreal h = sceneRect().height();
    qreal lon = y1 - ((h/2 + p.y())*abs(y1-y2)/h);
    qreal lat = x1 + ((w/2 + p.x())*abs(x1-x2)/w);
    latLon.setX(lat);
    latLon.setY(lon);
    return latLon;
    
}


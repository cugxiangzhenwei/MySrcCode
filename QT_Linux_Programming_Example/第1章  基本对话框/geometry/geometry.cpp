#include "geometry.h"

Geometry::Geometry()
{
    setWindowTitle(tr("Geometry"));

    label1 = new QLabel;
    label1->setText("x():"); 
    label2 = new QLabel;
    label2->setText("y():"); 
    label3 = new QLabel;
    label3->setText("frameGeometry():"); 
    label4 = new QLabel;
    label4->setText("pos():"); 
    label5 = new QLabel;
    label5->setText("geometry():"); 
    label6 = new QLabel;
    label6->setText("width():"); 
    label7 = new QLabel;
    label7->setText("height():"); 
    label8 = new QLabel;
    label8->setText("rect():"); 
    label9 = new QLabel;
    label9->setText("size():");  
    
    xLabel = new QLabel;
    yLabel = new QLabel;
    frameGeoLabel = new QLabel;
    posLabel = new QLabel;
    geoLabel = new QLabel;
    widthLabel = new QLabel;
    heightLabel = new QLabel;
    rectLabel = new QLabel;
    sizeLabel = new QLabel;  
    
    QGridLayout *layout = new QGridLayout(this);
    layout->setMargin(20);
    layout->setSpacing(10);
    int l = 0;
    int c = 1;
    layout->addWidget(label1,0,l);
    layout->addWidget(xLabel,0,c);
    layout->addWidget(label2,1,l);
    layout->addWidget(yLabel,1,c);
    layout->addWidget(label3,2,l);
    layout->addWidget(frameGeoLabel,2,c);
    layout->addWidget(label4,3,l);
    layout->addWidget(posLabel,3,c);
    layout->addWidget(label5,4,l);
    layout->addWidget(geoLabel,4,c);
    layout->addWidget(label6,5,l);
    layout->addWidget(widthLabel,5,c);
    layout->addWidget(label7,6,l);
    layout->addWidget(heightLabel,6,c);
    layout->addWidget(label8,7,l);
    layout->addWidget(rectLabel,7,c);
    layout->addWidget(label9,8,l);
    layout->addWidget(sizeLabel,8,c);
  
    updateLabel();
}

void
Geometry::updateLabel()
{
    QString temp;
    QString str_x;
    xLabel->setText(str_x.setNum(x()));
    QString str_y;
    yLabel->setText(str_y.setNum(y()));
    QString frameGeo;
    
    frameGeo = temp.setNum(frameGeometry().x()) + ", " + temp.setNum(frameGeometry().y()) + ", " + 
    			temp.setNum(frameGeometry().width()) + ", " + temp.setNum(frameGeometry().height());
    frameGeoLabel->setText(frameGeo);
    
    QString position ;
    position = temp.setNum(pos().x()) + ", " + temp.setNum(pos().y());
    posLabel->setText(position);
    
    QString geo;
    geo = temp.setNum(geometry().x()) + ", " + temp.setNum(geometry().y()) + ", " +
    		temp.setNum(geometry().width()) + ", " + temp.setNum(geometry().height());
    geoLabel->setText(geo);
    
    QString w;
    widthLabel->setText(w.setNum(width()));
    QString h;
    heightLabel->setText(h.setNum(height()));
    
    QString r;
    r = temp.setNum(rect().x()) + ", " + temp.setNum(rect().y()) + ", " +
    		temp.setNum(rect().width()) + ", " + temp.setNum(rect().height());
    rectLabel->setText(r);
    QString s;
    s = temp.setNum(size().width()) + ", " + temp.setNum(size().height());
    sizeLabel->setText(s);
}

void 
Geometry::moveEvent(QMoveEvent *)
{
    updateLabel();
}

void
Geometry::resizeEvent(QResizeEvent *)
{
    updateLabel();
}


#include "paintarea.h"
#include <math.h>

#define PI 3.1415926535

PaintArea::PaintArea(QWidget *parent)
{
    setPalette(QPalette(Qt::white));
    setAutoFillBackground(true);
    
    setMinimumSize(400,400);
    
    scale = 1;
    angle = 0;
    translate = 0;

}

void PaintArea::setScale(int s)
{
    scale = s/5.0;
    update();
}

void PaintArea::setRotate(int r)
{
    angle = r;
    update();
}

void PaintArea::setTranslate(int t)
{
    translate = t;
    update();
}


void PaintArea::paintEvent(QPaintEvent *)
{ 
    QPainter p(this);   
    
    QPainterPath path;
    path.addRect(-100,-50,200,100);
            
    p.translate(200,200);	//  把窗体的坐标原点移到中心点
    p.rotate(angle);
    p.scale(scale,scale);
    p.translate(translate,translate);

 
    p.setPen(Qt::red);
    p.drawLine(0, 0, 150, 0);
    p.drawLine(148, -2, 150, 0);
    p.drawLine(148, 2, 150, 0);
    p.drawText(150, 2, tr("x"));

    p.drawLine(0, 0, 0, 150);
    p.drawLine(-2, 148, 0, 150);
    p.drawLine(2, 148, 0, 150);
    p.drawText(2, 150, tr("y"));
    p.drawText(-10,-6,"(0,0)");   
    
    p.setPen(Qt::blue);
    p.drawPath(path); 
}

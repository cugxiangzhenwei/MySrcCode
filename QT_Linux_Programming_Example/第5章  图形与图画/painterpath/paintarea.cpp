#include "paintarea.h"
#include <math.h>

#define PI 3.1415926535

PaintArea::PaintArea(QWidget *parent)
{
    setPalette(QPalette(Qt::white));
    setAutoFillBackground(true);
    
    setMinimumSize(400,400);
    
    width = 1;
    penColor = Qt::red;
    brushColor = Qt::blue;
    rule = Qt::OddEvenFill;

}

void PaintArea::setFillRule(int index)
{
    rule = Qt::FillRule(index);
    update();
}

void PaintArea::setPenWidth(int w)
{
    width = w;
    update();
}

void PaintArea::setPenColor(QColor c)
{
    penColor = c;
    update();
}

void PaintArea::setBrushColor(QColor c)
{
    brushColor = c;
    update();
}

void PaintArea::paintEvent(QPaintEvent *)
{ 
    QPainter p(this);
    
    QPainterPath path;
    path.addRect(150,150,100,100);
    /*
    path.moveTo(150,150);
    path.lineTo(150,250);
    path.lineTo(250,250);
    path.lineTo(250,150);    
    path.closeSubpath();*/
    
    path.moveTo(100,100);
    path.cubicTo(300,100,200,200,300,300);
    path.cubicTo(100,300,200,200,100,100);
 
    path.setFillRule(rule);
    
    QPen pen;
    pen.setColor(penColor);
    pen.setWidth(width);
    p.setPen(pen);
    p.setBrush(QBrush(brushColor));
    
    p.drawPath(path);
}

#include "paintarea.h"
#include <math.h>

#define PI 3.1415926535

PaintArea::PaintArea(MainWidget *parent)
{
    setPalette(QPalette(Qt::white));
    setAutoFillBackground(true);
    
    setMinimumSize(400,400);
    
    mainWidget = parent;

    startPoint = QPoint(0,0);
    endPoint = QPoint(400,400);
}


void 
PaintArea::mousePressEvent(QMouseEvent * e)
{
    startPoint = e->pos();
}

void 
PaintArea::mouseReleaseEvent(QMouseEvent * e)
{
    endPoint = e->pos();
    isMouse = true;
    update();
}

void 
PaintArea::paintEvent(QPaintEvent *)
{ 
    QPainter p(this);
    QRect r = rect();
    if (mainWidget->style == Qt::LinearGradientPattern) 
    {
        QLinearGradient linearGradient(startPoint,endPoint);
        linearGradient.setColorAt(0.0, mainWidget->startColor);
        linearGradient.setColorAt(1.0, mainWidget->endColor);
        linearGradient.setSpread(mainWidget->spread);
        p.setBrush(linearGradient);
    } 
    else if (mainWidget->style == Qt::RadialGradientPattern) 
    {
        int r = sqrt(pow((double)(endPoint.x()-startPoint.x()),2) + pow(double(endPoint.y()-startPoint.y()),2));
        QRadialGradient radialGradient(startPoint, r, startPoint);
        radialGradient.setColorAt(0.0, mainWidget->startColor);
        radialGradient.setColorAt(1.0, mainWidget->endColor);
        radialGradient.setSpread(mainWidget->spread);
        p.setBrush(radialGradient);
    } 
    else if (mainWidget->style == Qt::ConicalGradientPattern) 
    {
        double angle = atan2(double(endPoint.y()-startPoint.y()), double(endPoint.x()-startPoint.x()));

        QConicalGradient conicalGradient(startPoint, -(180*angle)/PI);
        conicalGradient.setColorAt(0.0, mainWidget->startColor);
        conicalGradient.setColorAt(1.0, mainWidget->endColor);
        p.setBrush(conicalGradient);
    }
 
    p.drawRect(r);

}

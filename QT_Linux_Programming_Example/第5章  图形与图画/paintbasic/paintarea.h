#ifndef PAINTAREA_H
#define PAINTAREA_H

#include <QtGui>

class PaintArea : public QWidget
{
    Q_OBJECT
public:
    enum Shape {Line,Rectangle,RoundRect,Ellipse,Polygon,Polyline,Points,Arc,Path,Text,Pixmap};
    PaintArea(QWidget *parent=0);
     
    void setShape(Shape);
    void setPen(QPen);
    void setBrush(QBrush);
    
    void paintEvent(QPaintEvent *);

private:
    Shape shape;
    
    QBrush brush;
    QPen pen;   
};

#endif 

#ifndef PAINTAREA_H
#define PAINTAREA_H

#include <QtGui>
#include "mainwidget.h"

class MainWidget;

class PaintArea : public QWidget
{
    Q_OBJECT
public:

    PaintArea(MainWidget *parent);
     
   
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);
  
private:
    MainWidget *mainWidget;
    QPoint startPoint;
    QPoint endPoint;
    QColor startColor;
    QColor endColor;
    Qt::BrushStyle style;  
    
    bool isMouse;
};

#endif 

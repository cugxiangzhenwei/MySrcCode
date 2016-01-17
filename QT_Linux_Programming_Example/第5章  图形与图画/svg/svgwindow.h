#ifndef SVGWINDOW_H
#define SVGWINDOW_H

#include "svgwidget.h"

class SvgWindow : public QScrollArea
{
    Q_OBJECT
public:
    SvgWindow(QWidget *parent=0); 
    
    void setFile(QString);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);    
    
private:
    SvgWidget *svgWidget;
    
    QPoint mousePressPos;
    QPoint scrollBarValuesOnMousePress;
};

#endif
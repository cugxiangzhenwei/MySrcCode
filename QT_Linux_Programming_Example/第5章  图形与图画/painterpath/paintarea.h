#ifndef PAINTAREA_H
#define PAINTAREA_H

#include <QtGui>

class PaintArea : public QWidget
{
    Q_OBJECT
public:

    PaintArea(QWidget *parent=0);     
   
    void setFillRule(int);
    void setPenWidth(int);
    void setPenColor(QColor);
    void setBrushColor(QColor);
    
    void paintEvent(QPaintEvent *);
  
private:
    Qt::FillRule rule;
    int width;
    QColor penColor;
    QColor brushColor;
};

#endif 

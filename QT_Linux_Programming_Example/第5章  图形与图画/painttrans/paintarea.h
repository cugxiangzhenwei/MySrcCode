#ifndef PAINTAREA_H
#define PAINTAREA_H

#include <QtGui>

class PaintArea : public QWidget
{
    Q_OBJECT
public:

    PaintArea(QWidget *parent=0);     
   
    void setScale(int);
    void setTranslate(int);
    void setRotate(int);
    
    void paintEvent(QPaintEvent *);
  
private:
    qreal scale;
    int translate;
    int angle;

};

#endif 

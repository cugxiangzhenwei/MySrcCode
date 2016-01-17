#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "items.h"
#include <QWidget>

class QGraphicsView;
class QPixmap;
class QFrame;

class MainWidget : public QWidget
{
    Q_OBJECT
public:
    MainWidget(QWidget *parent=0);
    
    void createControllFrame();
    
public slots:
    void slotRotate(int); 
    void slotScale(int); 
    void slotShear(int); 
    void slotTranslate(int);
    
private:
    QGraphicsView *view;
    QFrame *ctrlFrame;
    PixItem *item;
    int angle;
    qreal scale;
    qreal shear;
    qreal translate;
   
};

#endif 	// MAINWIDGET_H

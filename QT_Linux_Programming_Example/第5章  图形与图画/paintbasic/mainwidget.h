#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QtGui>
#include "paintarea.h"

class MainWidget : public QWidget
{
    Q_OBJECT
public:
    MainWidget(QWidget *parent=0);
    
public slots:
    void slotShape(int); 
    void slotPenWidth(int); 
    void slotPenColor();
    void slotPenStyle(int);
    void slotPenCap(int); 
    void slotPenJoin(int);
    void slotBrush(int);
    
private:
    PaintArea *area;

    QComboBox *shapeComboBox;
    QSpinBox *widthSpinBox;
    QComboBox *penStyleComboBox;
    QComboBox *penCapComboBox;
    QComboBox *penJoinComboBox;
    QComboBox *brushStyleComboBox;
    QFrame *colorFrame;
};

#endif 	// MAINWIDGET_H

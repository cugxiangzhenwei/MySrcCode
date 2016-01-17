#ifndef RUBBERBAND_H
#define RUBBERBAND_H

#include <QtGui>

class QRubberBand;

class RubberBand : public QMainWindow
{
    Q_OBJECT
public:
    RubberBand(QWidget *parent=0);
    
    void mousePressEvent(QMouseEvent *);	// event
    void mouseMoveEvent(QMouseEvent *);		// event
    void mouseReleaseEvent(QMouseEvent *);	// event

private:
    QRubberBand *rubberBand;  
    QPoint origin;
};

#endif 	// RUBBERBAND_H

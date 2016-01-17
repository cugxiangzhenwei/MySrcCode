#ifndef MOUSEEVENT_H
#define MOUSEEVENT_H

#include <QtGui>

class MouseEvent : public QMainWindow
{
    Q_OBJECT
public:
    MouseEvent();
    ~MouseEvent();
private:    
    QLabel *labelStatus; 
    QLabel *labelMousePos;     
protected :
    void mouseMoveEvent ( QMouseEvent * e );
    void mousePressEvent ( QMouseEvent * e );
    void mouseReleaseEvent ( QMouseEvent * e );
    void mouseDoubleClickEvent( QMouseEvent * e );
};


#endif 

#ifndef DIGICLOCK_H
#define DIGICLOCK_H

#include <QtGui>

class DigiClock : public QLCDNumber
{
    Q_OBJECT
public:
    DigiClock(QWidget *parent=0);
    
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);

public slots:   
    void showTime();
    
private:
    QPoint dragPosition;
    bool showColon;
    
};

#endif

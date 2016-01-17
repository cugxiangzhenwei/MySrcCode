#include "digiclock.h"

DigiClock::DigiClock(QWidget *parent)
	: QLCDNumber(parent)
{
    QPalette p = palette();
    p.setColor(QPalette::Window,Qt::blue);
    setPalette(p);
    
    setWindowFlags(Qt::FramelessWindowHint);

    setWindowOpacity(0.5);
    
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
    timer->start(500);

    showTime();

    resize(150, 60);

	showColon=true;
}

void DigiClock::showTime()
{
     QTime time = QTime::currentTime();
     QString text = time.toString("hh:mm");
     if(showColon)
     {
     	text[2] = ':';
     	showColon = false;
     }
     else
     {
     	text[2] = ' ';
     	showColon = true;     	
     }
     display(text);
}
 
void DigiClock::mousePressEvent(QMouseEvent * e)
{
    if (e->button() == Qt::LeftButton)
    {
    	dragPosition = e->globalPos() - frameGeometry().topLeft();
    	e->accept();
    }
    if (e->button() == Qt::RightButton)
    {
    	close();
    }    
}

void DigiClock::mouseMoveEvent(QMouseEvent * e)
{
    if (e->buttons() & Qt::LeftButton)
    {
    	move(e->globalPos() - dragPosition);
    	e->accept();
    }
}


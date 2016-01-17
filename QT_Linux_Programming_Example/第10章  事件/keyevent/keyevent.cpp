#include "keyevent.h"

KeyEvent::KeyEvent(QWidget *parent)
	:QWidget(parent)
{  
    setWindowTitle(tr("Key Event"));
    
    setAutoFillBackground(true);
	
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window,Qt::white);
    setPalette(palette);

    setMinimumSize(512,256);
    setMaximumSize(512,256);
    
    width=size().width();
    height=size().height();
    
    pix = new QPixmap(width,height);
    pix->fill(Qt::white);
    
    step=32;
    image.load(":/images/1.png");
    
    startX=0;
    startY=0;
    
    drawPix();
}

KeyEvent::~KeyEvent()
{
}
             
void KeyEvent::drawPix()
{
	pix->fill(Qt::white);
	QPainter *painter = new QPainter(pix);
	QPen pen(Qt::DotLine);
    painter->setPen(pen);
    for(int i=step;i<width;)
    {
    	painter->drawLine(QPoint(i,0),QPoint(i,height));
    	i=i+step;
    }
    for(int j=step;j<height;)
    {
    	painter->drawLine(QPoint(0,j),QPoint(width,j));
    	j=j+step;
    }    
    painter->drawImage(QPoint(startX,startY),image);
}             
              
void KeyEvent::keyPressEvent(QKeyEvent *event)
{
	if(event->modifiers() == Qt::ControlModifier)
	{
		if(event->key() == Qt::Key_Left)
		{
			startX=(startX-1<0)?startX:startX-1;
			
		}
		if(event->key() == Qt::Key_Right)
		{
			startX=(startX+1+image.width ()>width)?startX:startX+1;
			
		}
		if(event->key() == Qt::Key_Up)
		{
			startY=(startY-1<0)?startY:startY-1;
			
		}	
		if(event->key() == Qt::Key_Down)
		{
			startY=(startY+1+image.height()>height)?startY:startY+1;
			
		}		
	}
	else
	{
		startX=startX-startX%step;
		startY=startY-startY%step;
		
		if(event->key() == Qt::Key_Left)
		{
			startX=(startX-step<0)?startX:startX-step;		
		}
		if(event->key() == Qt::Key_Right)
		{
			startX=(startX+step+image.width()>width)?startX:startX+step;		
		}
		if(event->key() == Qt::Key_Up)
		{
			startY=(startY-step<0)?startY:startY-step;		
		}	
		if(event->key() == Qt::Key_Down)
		{
			startY=(startY+step+image.height()>height)?startY:startY+step;
		}		
		if(event->key() == Qt::Key_Home)
		{
			startX=0;
			startY=0;
		}	
		if(event->key() == Qt::Key_End)
		{
			startX=width-image.width();
			startY=height-image.height();
		}			
	}
	drawPix();
	update();	
}              
                                                                             
void KeyEvent::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(QPoint(0,0),*pix);
}

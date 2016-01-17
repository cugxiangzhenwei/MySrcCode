#include "graphicsitem.h"
#include <QtGui>

//   Flash Item
FlashItem::FlashItem()
{   
    flash = true;
    setFlag(ItemIsMovable);
    
    startTimer(50);
}

QRectF
FlashItem::boundingRect() const
{
    qreal adjust = 2;
    return QRectF(-10 - adjust, -10 - adjust,
                   43 + adjust, 43 + adjust);
}

void
FlashItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::darkGray);
    painter->drawEllipse(-7,-7,40,40);
    
    painter->setPen(QPen(Qt::black,0));
    painter->setBrush(flash ? (Qt::red):(Qt::yellow));
    painter->drawEllipse(-10,-10,40,40);

}

void
FlashItem::timerEvent(QTimerEvent * )
{
    flash = !flash;
    update();
}


//  Star Item
StarItem::StarItem()
{
    pix.load(":/images/image.png");
}

QRectF
StarItem::boundingRect() const
{
    return QRectF(-pix.width()/2,-pix.height()/2,pix.width(),pix.height());
}

void
StarItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawPixmap(boundingRect().topLeft(),pix);
}





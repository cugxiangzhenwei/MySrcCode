#ifndef ITEMS_H
#define ITEMS_H

#include <QGraphicsItem>

class QGraphicsSceneMouseEvent;

class PixItem : public QGraphicsItem , public QObject
{
public:
    PixItem(QPixmap *pixmap);
    
    
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    
    void timerEvent(QTimerEvent *);
    void mousePressEvent(QGraphicsSceneMouseEvent *);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *);
    
private:
    QPixmap pix;

    QTimer *timer;
};


#endif 	// ITEMS_H

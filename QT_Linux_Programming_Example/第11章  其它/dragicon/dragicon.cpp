#include "dragicon.h"

DragIcon::DragIcon(QPixmap pix, QWidget *parent)
	: QLabel(parent)
{
     setScaledContents(true);
     setPixmap(pix);
}

void DragIcon::mousePressEvent(QMouseEvent * e)
{
    if(e->button() == Qt::LeftButton)
        startPos = e->pos();
}

void DragIcon::mouseMoveEvent(QMouseEvent * e)
{
    if (!e->buttons()&Qt::LeftButton)
    	return;
    if ((e->pos() - startPos).manhattanLength() < QApplication::startDragDistance())
    	return;

    QPixmap pix = *pixmap(); 
     
    QByteArray data;
    QDataStream stream(&data,QIODevice::WriteOnly);
    stream << pix << QPoint(e->pos()-rect().topLeft());
    QMimeData *mimeData = new QMimeData;
    mimeData->setData("Drag-Icon",data);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setHotSpot(QPoint(e->pos() - rect().topLeft()));
    drag->setPixmap(pix);
     
    hide();

    Qt::DropAction dropAction = drag->start(Qt::CopyAction | Qt::MoveAction);
  
    if (dropAction == Qt::MoveAction) 
        close();
    else
        show();
}



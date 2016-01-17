#include "dragwidget.h"
#include "dragicon.h"

DragWidget::DragWidget(QWidget *parent)
	: QWidget(parent)
{
    setMinimumSize(400,400);
    setAcceptDrops(true);

    DragIcon *icon1 = new DragIcon(QPixmap(":/images/sheep.png"),this);
    DragIcon *icon2 = new DragIcon(QPixmap(":/images/heart.png"),this);
    DragIcon *icon3 = new DragIcon(QPixmap(":/images/fish.png"),this);
    
    icon1->move(20,20);
    icon2->move(120,20);
    icon3->move(220,20);
}

void DragWidget::dragEnterEvent(QDragEnterEvent * e)
{
    if (e->mimeData()->hasFormat("Drag-Icon"))
    {
    	if(children().contains(e->source()))
    	{
    	    e->setDropAction(Qt::MoveAction);
      	    e->accept();
	}
	else
	    e->acceptProposedAction();
    }
}

void DragWidget::dragMoveEvent(QDragMoveEvent * e)
{
    if (e->mimeData()->hasFormat("Drag-Icon"))
    {
    	if(children().contains(e->source()))
    	{
    	    e->setDropAction(Qt::MoveAction);
      	    e->accept();
	}
	else
	    e->acceptProposedAction();
    }
}

void DragWidget::dropEvent(QDropEvent * e)
{
    if (e->mimeData()->hasFormat("Drag-Icon"))
    {
    	QByteArray data = e->mimeData()->data("Drag-Icon");
    	QDataStream stream(&data,QIODevice::ReadOnly);
    	QPixmap pix;
    	QPoint offset;
    	stream >> pix >> offset;
    	
    	DragIcon *icon = new DragIcon(pix,this);
    	icon->move(e->pos() - offset);
    	icon->show();
    	
    	if (children().contains(e->source()))
	{
	    e->setDropAction(Qt::MoveAction);
	    e->accept();
	}
	else
	    e->acceptProposedAction();
    }
    else
    	e->ignore();
}


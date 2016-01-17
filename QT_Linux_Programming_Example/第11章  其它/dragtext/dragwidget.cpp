#include "dragwidget.h"
#include "draglabel.h"
#include <QtGui>

DragWidget::DragWidget(QWidget *parent)
	: QFrame(parent)
{
    setMinimumSize(300,300);
    setAcceptDrops(true);
    setFrameStyle(QFrame::StyledPanel|QFrame::Raised);
    
}

void DragWidget::dragEnterEvent(QDragEnterEvent * e)
{
    if (e->mimeData()->hasText())
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
    if (e->mimeData()->hasText())
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
    if (e->mimeData()->hasFormat("Drag-Text"))
    {
    	QByteArray data = e->mimeData()->data("Drag-Text");
    	QDataStream stream(&data,QIODevice::ReadOnly);
    	QString text;
    	QPoint offset;
    	stream >> text >> offset;
    	
    	DragLabel *label = new DragLabel(text,this);
    	label->move(e->pos() - offset);
    	label->show();
    	
    	if (children().contains(e->source()))
	{
	    e->setDropAction(Qt::MoveAction);
	    e->accept();
	}
	else
	    e->acceptProposedAction();
    }
    else if (e->mimeData()->hasText())
    {
    	QStringList strList = e->mimeData()->text().split(QRegExp("\\s+"),QString::SkipEmptyParts);
    	QPoint pos = e->pos();
    	
    	foreach(QString str, strList)
    	{
	    DragLabel *dragLabel = new DragLabel(str,this);
	    dragLabel->move(pos);
	    dragLabel->show();
	    pos += QPoint(dragLabel->width(),0);
	}
	
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


#include "draglabel.h"
#include <QtGui>

DragLabel::DragLabel(const QString &text, QWidget *parent)
	: QLabel(text,parent)
{
     setAutoFillBackground(true);
     setFrameShape(QFrame::Panel);
     setFrameShadow(QFrame::Raised);
}

void DragLabel::mousePressEvent(QMouseEvent * e)
{
     
     QString str = text(); 
     QPixmap pix;
     pix = pix.grabWidget(this);

     QByteArray data;
     QDataStream stream(&data,QIODevice::WriteOnly);
     stream << str << QPoint(e->pos()-rect().topLeft());
     QMimeData *mimeData = new QMimeData;
     mimeData->setData("Drag-Text",data);
     mimeData->setText(str);

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


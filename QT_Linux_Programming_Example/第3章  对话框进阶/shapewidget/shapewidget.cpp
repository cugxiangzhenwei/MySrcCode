#include "shapewidget.h"

ShapeWidget::ShapeWidget(QWidget *parent)
	: QWidget(parent,Qt::FramelessWindowHint)
{
    QPixmap pix;
    pix.load(":/images/tux.png",0,Qt::AvoidDither|Qt::ThresholdDither|Qt::ThresholdAlphaDither);
    resize(pix.size());
    setMask(pix.mask());
}

void ShapeWidget::mousePressEvent(QMouseEvent * event)
{
    if (event->button() == Qt::LeftButton) 
    {
         dragPosition = event->globalPos() - frameGeometry().topLeft();
         event->accept();
    }
    if (event->button() == Qt::RightButton) 
    {
         close();
    }
}

void ShapeWidget::mouseMoveEvent(QMouseEvent * event)
{
    if (event->buttons() & Qt::LeftButton) 
    {
         move(event->globalPos() - dragPosition);
         event->accept();
    }
}

void ShapeWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,QPixmap(":/images/tux.png"));   
}


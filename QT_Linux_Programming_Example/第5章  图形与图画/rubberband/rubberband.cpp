#include "rubberband.h"

RubberBand::RubberBand(QWidget *parent)
	: QMainWindow(parent)
{
    QWidget *mainWidget = new QWidget;
    mainWidget->setBackgroundRole(QPalette::Light);
    mainWidget->setAutoFillBackground(true);
    setCentralWidget(mainWidget);
    
    resize(600,400);
    setWindowTitle(tr("Rubberband"));
    rubberBand = NULL;
}

void RubberBand::mousePressEvent(QMouseEvent * e)
{
    origin = e->pos();
    if (!rubberBand)
         rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
    rubberBand->setGeometry(QRect(origin, QSize()));
    rubberBand->show();
}

void RubberBand::mouseMoveEvent(QMouseEvent * e)
{
    if (rubberBand)
         rubberBand->setGeometry(QRect(origin, e->pos()).normalized());
}

void RubberBand::mouseReleaseEvent(QMouseEvent * e)
{
    if (rubberBand)
         rubberBand->hide();
}
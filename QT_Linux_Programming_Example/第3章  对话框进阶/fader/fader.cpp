#include "fader.h"
#include <QtGui>

FaderWidget::FaderWidget(QWidget *parent)
        : QWidget(parent)
{
    if (parent)
        startColor = parent->palette().window().color();
    else
        startColor = Qt::white;
    
    currentAlpha = 0;
    duration = 1000;
    
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()),this, SLOT(update()));
    
    setAttribute(Qt::WA_DeleteOnClose);
    resize(parent->size());
}


void FaderWidget::start()
{
    currentAlpha = 255;
    timer->start(100);
    show();
}

void FaderWidget::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);
    QColor semiTransparentColor = startColor;
    semiTransparentColor.setAlpha(currentAlpha);
    painter.fillRect(rect(), semiTransparentColor);
    
    currentAlpha -= 255 * timer->interval() / duration;
    if (currentAlpha <= 0) 
    {
        timer->stop();
        close();
    }
}

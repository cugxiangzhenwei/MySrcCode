#ifndef SVGWIDGET_H
#define SVGWIDGET_H

#include <QtGui>
#include <QtSvg>

class SvgWidget : public QSvgWidget
{
    Q_OBJECT
public:
    SvgWidget(QWidget *parent=0);
    void wheelEvent(QWheelEvent *);
    
private:
    QSvgRenderer * render;
};

#endif

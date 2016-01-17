#ifndef DRAWWIDGET_H
#define DRAWWIDGET_H

#include <QWidget>

class QPoint;
class QPixmap;
class QColor;

class DrawWidget : public QWidget
{
    Q_OBJECT
public:
    DrawWidget();
    
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);

public slots:   
    void setStyle(int);
    void setWidth(int);
    void setColor(QColor);
    void clear();
    
private:
    QPixmap *pix;
    QPoint startPos; 
    QPoint endPos;
    int style;
    int weight;
    QColor color;
};

#endif 

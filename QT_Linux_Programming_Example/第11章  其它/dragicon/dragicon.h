#ifndef DRAGICON_H
#define DRAGICON_H

#include <QtGui>

class DragIcon : public QLabel
{
    Q_OBJECT
public:
    DragIcon(QPixmap pix, QWidget *parent=0);
    
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);

private:
    QPoint startPos;
};

#endif 	// DRAGICON_H

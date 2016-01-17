#ifndef DRAGWIDGET_H
#define DRAGWIDGET_H
#include <QtGui>

class DragWidget : public QWidget
{
    Q_OBJECT
public:
    DragWidget(QWidget *parent=0);
    
    void dragEnterEvent(QDragEnterEvent *);
    void dragMoveEvent(QDragMoveEvent *);
    void dropEvent(QDropEvent *);
};

#endif 

#ifndef DRAGWIDGET_H
#define DRAGWIDGET_H

#include <QFrame>

class QDragEnterEvent;
class QDropEvent;

class DragWidget : public QFrame
{
    Q_OBJECT
public:
    DragWidget(QWidget *parent=0);
    
    void dragEnterEvent(QDragEnterEvent *);
    void dragMoveEvent(QDragMoveEvent *);
    void dropEvent(QDropEvent *);
};

#endif 	// DRAGWIDGET_H

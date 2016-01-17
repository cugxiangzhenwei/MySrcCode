#ifndef DRAGLABEL_H
#define DRAGLABEL_H

#include <QLabel>

class DragLabel : public QLabel
{
    Q_OBJECT
public:
    DragLabel(const QString &text, QWidget *parent=0);
    
    void mousePressEvent(QMouseEvent *);
};

#endif 	// DRAGLABEL_H

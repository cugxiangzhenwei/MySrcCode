#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QtGui>

#include "ui_first.h"
#include "ui_second.h"

class MyWidget : public QWidget
{
    Q_OBJECT
public:
    MyWidget(QWidget *parent=0);
    
public slots:
    void slotChild();
    
private:
    Ui::First firstUi;
    Ui::Second secondUi;    
};

#endif
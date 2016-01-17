#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QtGui>
#include "paintarea.h"

class MainWidget : public QWidget
{
    Q_OBJECT
public:
    MainWidget(QWidget *parent=0);
    
    void createCtrlWidget();

public slots:
    void slotRotate(int);
    void slotScale(int);
    void slotTranslate(int);
   
private:
    PaintArea *area;
    QFrame *ctrlFrame;
    
    QComboBox *fillRuleComboBox;
    QFrame *penColorFrame;
    QFrame *brushColorFrame;
};

#endif 	// MAINWIDGET_H

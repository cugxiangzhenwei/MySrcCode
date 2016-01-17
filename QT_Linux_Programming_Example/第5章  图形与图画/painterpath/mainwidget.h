#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "paintarea.h"

class MainWidget : public QWidget
{
    Q_OBJECT
public:
    MainWidget(QWidget *parent=0);
    
    void createCtrlWidget();

public slots:
    void slotPenColor();
    void slotBrushColor();
    void slotFillRule();
    void slotPenWidth(int);
   
private:
    PaintArea *area;
    QWidget *ctrlWidget;
    
    QComboBox *fillRuleComboBox;
    QFrame *penColorFrame;
    QFrame *brushColorFrame;
};

#endif 

#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QtGui>

class MainWidget : public QWidget
{
    Q_OBJECT
public:
    MainWidget(QWidget *parent=0);
    
    void createCtrlWidget();
    
    QColor startColor;
    QColor endColor;
    Qt::BrushStyle style;
    QGradient::Spread spread;
    
public slots:
    void slotStartColor();
    void slotEndColor();
    void slotSetStyle(int);
    void slotSetSpread(int);
   
private:
    QWidget *ctrlWidget;

    QPushButton *startPushButton;
    QPushButton *endPushButton;
    QComboBox *gradientComboBox;   
    QComboBox *spreadComboBox; 
};

#endif 	// MAINWIDGET_H

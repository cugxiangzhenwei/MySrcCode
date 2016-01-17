#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>

#include "svgwindow.h"

class MainWidget : public QMainWindow
{
    Q_OBJECT
public:
    MainWidget(QWidget *parent=0);
    
    void createMenu();
public slots:
    void slotOpenFile();
    
private:
    SvgWindow *svgWindow;
};

#endif 	// MAINWINDOW_H

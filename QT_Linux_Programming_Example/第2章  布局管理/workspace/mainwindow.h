#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>

class MainWidget : public QMainWindow
{
    Q_OBJECT
public:
    MainWidget(QWidget *parent=0);
    
    void createMenu();
    
public slots:
    void slotScroll(bool);
    
private:
    QWorkspace *workSpace;
};

#endif 

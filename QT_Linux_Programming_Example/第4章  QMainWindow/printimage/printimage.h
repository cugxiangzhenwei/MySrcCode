#ifndef PRINTIMAGE_H
#define PRINTIMAGE_H

#include <QtGui>


class PrintImage : public QMainWindow	
{
    Q_OBJECT
public:
    PrintImage();
    ~PrintImage();
    void createMenus();
    void createActions();
    void createToolBars();    
private:
    QImage image;
    QLabel *imageLabel;
    
    QMenu *printMenu;
    QAction *printAction;
    QToolBar *printTool;   
   
protected slots:
    void slotPrint();
};


#endif

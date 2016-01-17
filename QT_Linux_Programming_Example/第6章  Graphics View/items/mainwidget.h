#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "graphicsitem.h"
#include <QMainWindow>

class QAction;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent=0);
    
    void initScene();
    void createActions();
    void createMenus();
    
public slots:
    void slotNew(); 
    void slotClear();
    void slotAddFlashItem(); 
    void slotAddEllipseItem(); 
    void slotAddPolygonItem();
    void slotAddTextItem();
    void slotAddRectItem();
    void slotAddAlphaItem();
    void slotAddAnimationItem();
    
private:
    QGraphicsScene *scene;
    
    QAction *newAct;
    QAction *clearAct;
    QAction *exitAct;
    QAction *addFlashItemAct;
    QAction *addEllipseItemAct;
    QAction *addPolygonItemAct;
    QAction *addTextItemAct;
    QAction *addAlphaItemAct;
    QAction *addRectItemAct;
    QAction *addAnimItemAct;
};

#endif 	// MAINWIDGET_H

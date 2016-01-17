#ifndef IMGPROCESSOR_H
#define IMGPROCESSOR_H

#include <QtGui>


class ImgProcessor : public QMainWindow	
{
    Q_OBJECT
public:
    ImgProcessor();
    ~ImgProcessor();
    
    void createMenus();
    void createActions();
    void createToolBars();
private:
    QImage img;
    QLabel *imageLabel;
    
    QMenu *fileMenu;
    QMenu *zoomMenu;
    QMenu *rotateMenu;
    QMenu *mirrorMenu;

    QAction *openFileAction;
    QAction *zoomInAction;
    QAction *zoomOutAction;
    QAction *rotate90Action;
    QAction *rotate180Action;
    QAction *rotate270Action;
    QAction *mirrorVerticalAction;
    QAction *mirrorHorizontalAction;
    
    QToolBar *fileTool;
    QToolBar *zoomTool;  
    QToolBar *rotateTool;
    QToolBar *mirrorTool;
         
protected slots:
    void slotOpenFile();
    void slotZoomIn();
    void slotZoomOut();
    void slotRotate90();
    void slotRotate180();
    void slotRotate270();
    void slotMirrorVertical();
    void slotMirrorHorizontal();
};


#endif

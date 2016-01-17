#ifndef LISTALIGN_H
#define LISTALIGN_H

#include <QtGui>

class ListAlign : public QMainWindow
{
    Q_OBJECT
public:
    ListAlign(QWidget *parent=0);
     
public slots:
    void slotAlignment(QAction*);
    void slotList(int);
    void slotCursorPositionChanged();
        
private:
    QTextEdit *text;
    QLabel *label;
        
    QComboBox *listBox;
    QAction *leftAction;
    QAction *rightAction;
    QAction *centerAction;
    QAction *justifyAction;
    QAction *redoAction;
    QAction *undoAction;
};

#endif 

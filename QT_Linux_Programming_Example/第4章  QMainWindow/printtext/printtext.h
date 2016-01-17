#ifndef PRINTTEXT_H
#define PRINTTEXT_H

#include <QtGui>


class PrintText : public QMainWindow	
{
    Q_OBJECT
public:
    PrintText();
    ~PrintText();
    void createMenus();
    void createActions();
    void createToolBars();    
private:
    QTextEdit *contentTextEdit;
    
    QMenu *printMenu;
    QAction *printAction;
    QToolBar *printTool;   
    
protected slots:
    void slotPrint();
};


#endif

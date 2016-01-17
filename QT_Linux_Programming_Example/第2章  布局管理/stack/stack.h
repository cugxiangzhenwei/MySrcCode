#ifndef STACK_H
#define STACK_H

#include <QtGui>

class StackDlg : public QDialog
{
    Q_OBJECT
public:
    StackDlg(QWidget *parent = 0, Qt::WindowFlags f1 = 0);

    QLabel *label1;
    QLabel *label2;
    QLabel *label3;
    QListWidget *list;
    QStackedWidget *stack;
    
};

#endif

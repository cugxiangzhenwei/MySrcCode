#include "stack.h"

StackDlg::StackDlg(QWidget *parent, Qt::WindowFlags f1)
        : QDialog(parent,f1)
{
    setWindowTitle(tr("Stacked Widgets"));
    
    list = new QListWidget(this);
    list->insertItem(0,tr("Window 1"));
	list->insertItem(1,tr("Window 2"));
	list->insertItem(2,tr("Window 3"));
     
    label1 = new QLabel(tr("This is Window 1 !"));
    label2 = new QLabel(tr("This is Window 2 !"));
    label3 = new QLabel(tr("This is Window 3 !"));
       
    stack = new QStackedWidget(this);
    stack->addWidget(label1);
    stack->addWidget(label2);
    stack->addWidget(label3);
     
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setMargin(5);
    mainLayout->setSpacing(5);
    mainLayout->addWidget(list);
    mainLayout->addWidget(stack,0,Qt::AlignHCenter);
    mainLayout->setStretchFactor(list,1);
    mainLayout->setStretchFactor(stack,3);
    
    connect(list,SIGNAL(currentRowChanged(int)),stack,SLOT(setCurrentIndex(int)));
    
}



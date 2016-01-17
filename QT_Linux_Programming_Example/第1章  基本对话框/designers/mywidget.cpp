#include "mywidget.h"

MyWidget::MyWidget(QWidget *parent)
	: QWidget(parent)
{
	 
    
    QTabWidget *tabWidget = new QTabWidget(this);     
    
    QWidget *w1 = new QWidget;
    firstUi.setupUi(w1);  
    
    QWidget *w2 = new QWidget;
    secondUi.setupUi(w2);

    tabWidget->addTab(w1,"First");
    tabWidget->addTab(w2,"Second");
    
    tabWidget->resize(300,300);
    
    connect(firstUi.childPushButton,SIGNAL(clicked()),this,SLOT(slotChild())); 
    connect(secondUi.closePushButton,SIGNAL(clicked()),this,SLOT(close()));   
}

void MyWidget::slotChild()
{
    QDialog *dlg = new QDialog;
    thirdUi.setupUi(dlg);
    dlg->exec();
}
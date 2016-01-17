#include "mywidget.h"
#include <QtUiTools>

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
    QUiLoader loader;
    QFile file(":/forms/third.ui");
    file.open(QFile::ReadOnly);
    QWidget *third = loader.load(&file);
    file.close();

    third->show();
}
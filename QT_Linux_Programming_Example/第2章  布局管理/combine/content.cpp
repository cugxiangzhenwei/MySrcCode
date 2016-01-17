#include <QApplication>
#include <QLayout>
#include <QTextCodec>

#include "content.h"


Content::Content(QWidget *parent, Qt::WindowFlags f1)
	: QFrame(parent,f1)
{
    stack = new QStackedWidget();
    stack->setFrameStyle(QFrame::Panel|QFrame::Raised);
    baseInfo = new BaseInfo();
    contact = new Contact();
    detail = new Detail();
    stack->addWidget(baseInfo);
    stack->addWidget(contact);
    stack->addWidget(detail);
    
    pushButtonAmend = new QPushButton();
    pushButtonAmend->setText(tr("Demand"));
    pushButtonClose = new QPushButton();
    pushButtonClose->setText(tr("Close"));
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(pushButtonAmend);
    buttonLayout->addWidget(pushButtonClose);
    
    QVBoxLayout *mainlayout = new QVBoxLayout(this);
    mainlayout->setMargin(10);
    mainlayout->setSpacing(6);
    mainlayout->addWidget(stack);
    mainlayout->addLayout(buttonLayout);
    
    connect(pushButtonClose,SIGNAL(clicked()),qApp,SLOT(quit()));
}

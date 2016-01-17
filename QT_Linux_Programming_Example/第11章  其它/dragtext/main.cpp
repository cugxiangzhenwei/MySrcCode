#include <QApplication>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QWidget>
#include "dragwidget.h"

int 
main(int argc, char * argv[])
{
    QApplication app(argc,argv);
    
    QWidget *mainWidget = new QWidget;
    QTextEdit *edit = new QTextEdit;
    edit->setText("Since we will continue to improve and fine-tune Qt as a result of feedback and suggestions from customers and the open source community, we have not included a detailed list of changes between Qt 4.1 and this release.");
    DragWidget *dragWidget = new DragWidget;
    
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(edit);
    layout->addWidget(dragWidget);
    layout->setStretchFactor(edit,1);
    layout->setStretchFactor(dragWidget,3);
    mainWidget->setLayout(layout);
    mainWidget->show();
    
    return app.exec();
}

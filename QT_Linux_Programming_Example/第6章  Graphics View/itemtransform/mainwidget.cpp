#include "mainwidget.h"
#include <QtGui>

#include <math.h>

static const double PI = 3.14159265358979323846264338327950288419717;

MainWidget::MainWidget(QWidget *parent)
	: QWidget(parent)
{
    angle = 0;
    scale = 5;
    shear = 5;
    translate = 50;
    
    QGraphicsScene *scene = new QGraphicsScene;
    scene->setSceneRect(-200,-200,400,400);
    
    QPixmap *pixmap = new QPixmap(":/images/rotate.png");
    item = new PixItem(pixmap);
    
    scene->addItem(item);
    item->setPos(0,0);
    
    view = new QGraphicsView;
    view->setScene(scene);
    view->setMinimumSize(400,400);
    
    ctrlFrame = new QFrame;
    createControllFrame();
    
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setMargin(10);
    mainLayout->setSpacing(20);
    mainLayout->addWidget(view);
    mainLayout->addWidget(ctrlFrame);
    setLayout(mainLayout);
    
    setWindowTitle(tr("Graphics Item Transformation"));
}

void
MainWidget::createControllFrame()
{
   
    //  Rotate Controll
    QGroupBox *rotateGroup = new QGroupBox(tr("Rotate"));
    QSlider *rotateSlider = new QSlider;
    rotateSlider->setOrientation(Qt::Horizontal);
    rotateSlider->setRange(0,360);
    connect(rotateSlider,SIGNAL(valueChanged(int)),this,SLOT(slotRotate(int)));
    QHBoxLayout *l1 = new QHBoxLayout;
    l1->addWidget(rotateSlider);
    rotateGroup->setLayout(l1);
        
    //  Scale Controll
    QGroupBox *scaleGroup = new QGroupBox(tr("Scale"));
    QSlider *scaleSlider = new QSlider;
    scaleSlider->setOrientation(Qt::Horizontal);
    scaleSlider->setRange(1,10);
    scaleSlider->setValue(5);
    connect(scaleSlider,SIGNAL(valueChanged(int)),this,SLOT(slotScale(int)));
    QHBoxLayout *l2 = new QHBoxLayout;
    l2->addWidget(scaleSlider);
    scaleGroup->setLayout(l2);
    
    //  Shear Controll
    QGroupBox *shearGroup = new QGroupBox(tr("Shear"));
    QSlider *shearSlider = new QSlider;
    shearSlider->setOrientation(Qt::Horizontal);
    shearSlider->setRange(0,9);
    shearSlider->setValue(5);
    connect(shearSlider,SIGNAL(valueChanged(int)),this,SLOT(slotShear(int)));
    QHBoxLayout *l3 = new QHBoxLayout;
    l3->addWidget(shearSlider);
    shearGroup->setLayout(l3);
    
    //  Tranlate Controll
    QGroupBox *translateGroup = new QGroupBox(tr("Translate"));
    QSlider *translateSlider = new QSlider;
    translateSlider->setOrientation(Qt::Horizontal);
    translateSlider->setRange(0,100);
    translateSlider->setValue(50);
    connect(translateSlider,SIGNAL(valueChanged(int)),this,SLOT(slotTranslate(int)));
    QHBoxLayout *l4 = new QHBoxLayout;
    l4->addWidget(translateSlider);
    translateGroup->setLayout(l4);
    
    QVBoxLayout *ctrlLayout = new QVBoxLayout;
    ctrlLayout->addWidget(rotateGroup);
    ctrlLayout->addWidget(scaleGroup);
    ctrlLayout->addWidget(shearGroup);
    ctrlLayout->addWidget(translateGroup);
    ctrlLayout->addStretch();
    ctrlFrame->setLayout(ctrlLayout);
}

void
MainWidget::slotRotate(int value)
{
    item->rotate(value-angle);
    angle = value;
}

void
MainWidget::slotScale(int value)
{
    qreal s;
    if (value>scale)
    	s = pow(1.1,(value-scale));
    else
    	s = pow(1/1.1,(scale-value));
    	
    item->scale(s,s);
    
    scale = value;
}

void
MainWidget::slotShear(int value)
{

    item->shear((value-shear)/10.0,0);
    shear = value;

}


void
MainWidget::slotTranslate(int value)
{
    item->translate(value-translate,value-translate);
    translate = value;
}


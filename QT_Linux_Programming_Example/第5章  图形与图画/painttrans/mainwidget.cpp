#include "mainwidget.h"
#include "paintarea.h"

MainWidget::MainWidget(QWidget *parent)
	: QWidget(parent)
{
    QFont font("ZYSong18030",12);
    setFont(font);
    
    area = new PaintArea;
   
    ctrlFrame = new QFrame;
    createCtrlWidget();
    
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(area);
    mainLayout->addWidget(ctrlFrame);
    setLayout(mainLayout); 
    
    setWindowTitle(tr("Painter Transformation"));   
}
 
void MainWidget::createCtrlWidget()
{
    //  旋转控制
    QGroupBox *rotateGroup = new QGroupBox(tr("Rotate"));
    QSlider *rotateSlider = new QSlider;
    rotateSlider->setOrientation(Qt::Horizontal);
    rotateSlider->setRange(0,360);
    connect(rotateSlider,SIGNAL(valueChanged(int)),this,SLOT(slotRotate(int)));
    QHBoxLayout *l1 = new QHBoxLayout;
    l1->addWidget(rotateSlider);
    rotateGroup->setLayout(l1);
        
    //  缩放控制
    QGroupBox *scaleGroup = new QGroupBox(tr("Scale"));
    QSlider *scaleSlider = new QSlider;
    scaleSlider->setOrientation(Qt::Horizontal);
    scaleSlider->setRange(1,10);
    scaleSlider->setValue(5);
    connect(scaleSlider,SIGNAL(valueChanged(int)),this,SLOT(slotScale(int)));
    QHBoxLayout *l2 = new QHBoxLayout;
    l2->addWidget(scaleSlider);
    scaleGroup->setLayout(l2);   
   
    //  平移控制
    QGroupBox *translateGroup = new QGroupBox(tr("Translate"));
    QSlider *translateSlider = new QSlider;
    translateSlider->setOrientation(Qt::Horizontal);
    translateSlider->setRange(0,100);
    translateSlider->setValue(0);
    connect(translateSlider,SIGNAL(valueChanged(int)),this,SLOT(slotTranslate(int)));
    QHBoxLayout *l4 = new QHBoxLayout;
    l4->addWidget(translateSlider);
    translateGroup->setLayout(l4);
    
    QVBoxLayout *ctrlLayout = new QVBoxLayout;
    ctrlLayout->addWidget(rotateGroup);
    ctrlLayout->addWidget(scaleGroup);
    ctrlLayout->addWidget(translateGroup);
    ctrlLayout->addStretch();
    ctrlFrame->setLayout(ctrlLayout);
}

void MainWidget::slotRotate(int value)
{
    area->setRotate(value);
}

void MainWidget::slotScale(int value)
{
    area->setScale(value);
}

void MainWidget::slotTranslate(int value)
{
    area->setTranslate(value);
}







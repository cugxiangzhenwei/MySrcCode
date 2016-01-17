#include "mainwidget.h"
#include "paintarea.h"

MainWidget::MainWidget(QWidget *parent)
	: QWidget(parent)
{
    QFont font("ZYSong18030",12);
    setFont(font);
    
    startColor = Qt::green;
    endColor = Qt::blue;
    style = Qt::LinearGradientPattern; 
    
    PaintArea *area = new PaintArea(this);
   
    ctrlWidget = new QWidget;
    createCtrlWidget();
    
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(area);
    mainLayout->addWidget(ctrlWidget);
    setLayout(mainLayout); 
    
    setWindowTitle(tr("Gradient"));   
}
 
void
MainWidget::createCtrlWidget()
{
    startPushButton = new QPushButton(tr("start"));
    startPushButton->setAutoFillBackground(true);
    startPushButton->setPalette(QPalette(Qt::green));
    endPushButton = new QPushButton(tr("end"));
    endPushButton->setAutoFillBackground(true);
    endPushButton->setPalette(QPalette(Qt::blue));
    connect(startPushButton,SIGNAL(clicked()),this,SLOT(slotStartColor()));
    connect(endPushButton,SIGNAL(clicked()),this,SLOT(slotEndColor()));
    
    gradientComboBox = new QComboBox;
    gradientComboBox->addItem(tr("Linear Gradient"),Qt::LinearGradientPattern);
    gradientComboBox->addItem(tr("Radial Gradient"),Qt::RadialGradientPattern);
    gradientComboBox->addItem(tr("Conical Gradient"),Qt::ConicalGradientPattern);
    connect(gradientComboBox,SIGNAL(activated(int)),this,SLOT(slotSetStyle(int)));
    
    spreadComboBox = new QComboBox;
    spreadComboBox->addItem(tr("PadSpread"),QGradient::PadSpread);
    spreadComboBox->addItem(tr("RepeatSpread"),QGradient::RepeatSpread);
    spreadComboBox->addItem(tr("ReflectSpread"),QGradient::ReflectSpread);
    connect(spreadComboBox,SIGNAL(activated(int)),this,SLOT(slotSetSpread(int)));
             
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(startPushButton);
    layout->addWidget(endPushButton);
    layout->addWidget(gradientComboBox);
    layout->addWidget(spreadComboBox);
    layout->setMargin(10);
    layout->setSpacing(10);
    ctrlWidget->setLayout(layout);
}

void
MainWidget::slotStartColor()
{
    startColor = QColorDialog::getColor(Qt::green);
    startPushButton->setPalette(QPalette(startColor));

}

void
MainWidget::slotEndColor()
{
    endColor = QColorDialog::getColor(Qt::blue);
    endPushButton->setPalette(QPalette(endColor));

}

void
MainWidget::slotSetStyle(int value)
{
    style = Qt::BrushStyle(gradientComboBox->itemData(value, Qt::UserRole).toInt());
}

void
MainWidget::slotSetSpread(int value)
{
    spread = QGradient::Spread(spreadComboBox->itemData(value, Qt::UserRole).toInt());
}






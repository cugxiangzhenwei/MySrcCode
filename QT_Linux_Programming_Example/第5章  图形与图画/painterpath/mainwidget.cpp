#include "mainwidget.h"

MainWidget::MainWidget(QWidget *parent)
	: QWidget(parent)
{
    QFont font("ZYSong18030",12);
    setFont(font);
    
    area = new PaintArea;
   
    ctrlWidget = new QWidget;
    createCtrlWidget();
    
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(area);
    mainLayout->addWidget(ctrlWidget);
    setLayout(mainLayout); 
    
    setWindowTitle(tr("Painter Path"));   
}
 
void MainWidget::createCtrlWidget()
{
    QLabel *label1 = new QLabel(tr("Fill Rule:"));
    QLabel *label2 = new QLabel(tr("Pen Width:"));
    QLabel *label3 = new QLabel(tr("Pen Color:"));
    QLabel *label4 = new QLabel(tr("Brush Color:"));
    
    fillRuleComboBox = new QComboBox;
    fillRuleComboBox->addItem(tr("Odd Even"),Qt::OddEvenFill);
    fillRuleComboBox->addItem(tr("Winding"),Qt::WindingFill);
    connect(fillRuleComboBox,SIGNAL(activated(int)),this,SLOT(slotFillRule()));
    
    QSpinBox *widthSpinBox = new QSpinBox;
    widthSpinBox->setRange(1,20);
    connect(widthSpinBox,SIGNAL(valueChanged(int)),this,SLOT(slotPenWidth(int)));
    
    penColorFrame = new QFrame;
    penColorFrame->setAutoFillBackground(true);
    penColorFrame->setPalette(QPalette(Qt::red));
    QPushButton *penColorPushButton = new QPushButton(tr("Change"));
    connect(penColorPushButton,SIGNAL(clicked()),this,SLOT(slotPenColor()));
    
    brushColorFrame = new QFrame;
    brushColorFrame->setAutoFillBackground(true);
    brushColorFrame->setPalette(QPalette(Qt::blue));
    QPushButton *brushColorPushButton = new QPushButton(tr("Change"));
    connect(brushColorPushButton,SIGNAL(clicked()),this,SLOT(slotBrushColor()));    

    QGridLayout *grid = new QGridLayout;
    grid->addWidget(label1,0,0);
    grid->addWidget(fillRuleComboBox,0,1);
    grid->addWidget(label2,1,0);
    grid->addWidget(widthSpinBox,1,1);
    grid->addWidget(label3,2,0);
    grid->addWidget(penColorFrame,2,1);
    grid->addWidget(penColorPushButton,2,2);
    grid->addWidget(label4,3,0);
    grid->addWidget(brushColorFrame,3,1);
    grid->addWidget(brushColorPushButton,3,2);
    grid->setMargin(10);
    grid->setSpacing(10);
    
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(grid);
    layout->addStretch();
    ctrlWidget->setLayout(layout);    
}

void MainWidget::slotPenWidth(int value)
{
    area->setPenWidth(value);
}

void MainWidget::slotFillRule()
{
    area->setFillRule(fillRuleComboBox->itemData(
    		fillRuleComboBox->currentIndex(),Qt::UserRole).toInt());
}

void MainWidget::slotPenColor()
{
    QColor color = QColorDialog::getColor(Qt::red);
    area->setPenColor(color);
    penColorFrame->setPalette(QPalette(color));
}

void MainWidget::slotBrushColor()
{
    QColor color = QColorDialog::getColor(Qt::blue);
    area->setBrushColor(color);
    brushColorFrame->setPalette(QPalette(color));      
}






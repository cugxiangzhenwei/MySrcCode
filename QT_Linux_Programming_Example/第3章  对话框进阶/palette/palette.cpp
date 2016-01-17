#include "palette.h"

#include <QtGui>

Palette::Palette(QWidget *parent)
        : QDialog(parent)
{
    QFont font("ZYSong18030",12);
    setFont(font);
    
    setWindowTitle(tr("QPalette"));
    
    ctrlFrame = new QFrame;
    createCtrlFrame();
    contentFrame = new QFrame;
    contentFrame->setFrameStyle(QFrame::Panel|QFrame::Raised);
    contentFrame->setAutoFillBackground(true);
    createContentFrame();
    
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(ctrlFrame);
    layout->addWidget(contentFrame);
    layout->setMargin(10);
    layout->setSpacing(8);
    setLayout(layout);
}

void Palette::createCtrlFrame()
{
    QLabel *label1 = new QLabel(tr("QPalette::Window:"));
    QLabel *label2 = new QLabel(tr("QPalette::WindowText:"));
    QLabel *label3 = new QLabel(tr("QPalette::Button:"));
    QLabel *label4 = new QLabel(tr("QPalette::ButtonText:"));
    QLabel *label5 = new QLabel(tr("QPalette::Base:"));

    windowComboBox = new QComboBox;
    windowTextComboBox = new QComboBox;
    buttonComboBox = new QComboBox;
    buttonTextComboBox = new QComboBox;
    baseComboBox = new QComboBox;
    
    fillColorList(windowComboBox);
    fillColorList(windowTextComboBox);
    fillColorList(buttonComboBox);
    fillColorList(buttonTextComboBox);
    fillColorList(baseComboBox);
    
    connect(windowComboBox,SIGNAL(activated(int)),this,SLOT(slotWindow()));
    connect(windowTextComboBox,SIGNAL(activated(int)),this,SLOT(slotWindowText()));
    connect(buttonComboBox,SIGNAL(activated(int)),this,SLOT(slotButton()));
    connect(buttonTextComboBox,SIGNAL(activated(int)),this,SLOT(slotButtonText()));
    connect(baseComboBox,SIGNAL(activated(int)),this,SLOT(slotBase()));
    
    QGridLayout *grid = new QGridLayout;
    grid->addWidget(label1,0,0);
    grid->addWidget(label2,1,0);
    grid->addWidget(label3,2,0);
    grid->addWidget(label4,3,0);
    grid->addWidget(label5,4,0);
    grid->addWidget(windowComboBox,0,1);
    grid->addWidget(windowTextComboBox,1,1);
    grid->addWidget(buttonComboBox,2,1);
    grid->addWidget(buttonTextComboBox,3,1);
    grid->addWidget(baseComboBox,4,1);
    grid->setSpacing(8);
    
    ctrlFrame->setLayout(grid);
}

void Palette::createContentFrame()
{
    QLabel *label1 = new QLabel(tr("Please Select Value"));
    QSpinBox *spinBox =  new QSpinBox;
    spinBox->setRange(1,10);
    
    QLabel *label2 = new QLabel(tr("Please Input String:"));
    QLineEdit *lineEdit = new QLineEdit;
    
    QTextEdit *edit = new QTextEdit;
    edit->setText("Hello Qt!");
    
    QGridLayout *grid = new QGridLayout;
    grid->addWidget(label1,0,0);
    grid->addWidget(spinBox,0,1);
    grid->addWidget(label2,1,0);
    grid->addWidget(lineEdit,1,1);
    grid->addWidget(edit,2,0,1,2);
        
    QPushButton *okPushButton = new QPushButton(tr("OK"));
    okPushButton->setAutoFillBackground(true);
    QPushButton *cancelPushButton = new QPushButton(tr("Cancel"));
    cancelPushButton->setAutoFillBackground(true);
    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addStretch();
    hLayout->addWidget(okPushButton);
    hLayout->addWidget(cancelPushButton);
    
    QVBoxLayout *contentLayout = new QVBoxLayout;
    contentLayout->addLayout(grid);
    contentLayout->addLayout(hLayout);
    contentLayout->setMargin(10);
    contentLayout->setSpacing(8);
    
    contentFrame->setLayout(contentLayout);
}

void Palette::fillColorList(QComboBox * combobox)
{
    QStringList colorList = QColor::colorNames();
    
    QString color;
    foreach(color,colorList)
    {
	QPixmap pix(QSize(70,20));
    	pix.fill(QColor(color));
    	combobox->addItem(QIcon(pix),NULL);
    	combobox->setIconSize(QSize(70,20));
    	combobox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    }
}

void Palette::slotWindow()
{
    QStringList colorList = QColor::colorNames();
    QColor color = QColor(colorList[windowComboBox->currentIndex()]);
    QPalette p = contentFrame->palette();
    p.setColor(QPalette::Window,color);
    contentFrame->setPalette(p);
}

void Palette::slotWindowText()
{
    QStringList colorList = QColor::colorNames();
    QColor color = QColor(colorList[windowTextComboBox->currentIndex()]);
    QPalette p = contentFrame->palette();
    p.setColor(QPalette::WindowText,color);
    contentFrame->setPalette(p);
}

void Palette::slotButton()
{
    QStringList colorList = QColor::colorNames();
    QColor color = QColor(colorList[buttonComboBox->currentIndex()]);
    QPalette p = contentFrame->palette();
    p.setColor(QPalette::Button,color);
    contentFrame->setPalette(p);
}

void Palette::slotButtonText()
{
    QStringList colorList = QColor::colorNames();
    QColor color = QColor(colorList[buttonTextComboBox->currentIndex()]);
    QPalette p = contentFrame->palette();
    p.setColor(QPalette::ButtonText,color);
    contentFrame->setPalette(p);
}

void Palette::slotBase()
{
    QStringList colorList = QColor::colorNames();
    QColor color = QColor(colorList[baseComboBox->currentIndex()]);
    QPalette p = contentFrame->palette();
    p.setColor(QPalette::Base,color);
    contentFrame->setPalette(p);
}


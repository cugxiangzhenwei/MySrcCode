#include "toolbar.h"
#include <QtGui>

ToolBar::ToolBar()
{
    QFont f("ZYSong18030",10);
    setFont(f);
    
    setWindowTitle(tr("Insert a ComboBox to toolbar"));
    
    text =  new QTextEdit(this);
    text->setReadOnly(true);
    setCentralWidget(text);
    
    QToolBar *toolBar = addToolBar("control");
    QLabel *label1 = new QLabel(tr("ctrl1: "));
    box = new QComboBox;
    box->insertItem(0,tr("ComboBox 1"));
    box->insertItem(1,tr("ComboBox 2"));
    box->insertItem(2,tr("ComboBox 3"));
    toolBar->addWidget(label1);
    toolBar->addWidget(box);
    toolBar->addSeparator();
    QLabel *label2 = new QLabel(tr("ctrl2: "));
    spin = new QSpinBox;
    spin->setRange(1,10);
    toolBar->addWidget(label2);
    toolBar->addWidget(spin);
    
    connect(box,SIGNAL(currentIndexChanged(QString)),this,SLOT(slotComboBox(QString)));
    connect(spin,SIGNAL(valueChanged(QString)),this,SLOT(slotSpinBox(QString)));
}

void ToolBar::slotComboBox(QString combo)
{
    QString doc;
    QString spinStr;
    doc = "QComboBox: " + combo + "\n" + "QSpinBox: " + spinStr.setNum(spin->value());
    text->setText(doc);
}

void ToolBar::slotSpinBox(QString value)
{
    QString doc;
    doc = "QComboBox: " + box->currentText() + "\n" + "QSpinBox: " + value;
    text->setText(doc);
}

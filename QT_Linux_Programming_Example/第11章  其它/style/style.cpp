#include "style.h"

#include <QtGui>

Style::Style(QWidget *parent)
	:QDialog(parent)
{
    setWindowTitle(tr("Change Window Style"));
    
    createLeftLayout();
    createRightLayout();
    
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setMargin(10);
    mainLayout->setSpacing(5);
    mainLayout->addWidget(leftFrame);
    mainLayout->addWidget(rightFrame);
    setLayout(mainLayout);
}

void
Style::createLeftLayout()
{    
    leftFrame = new QFrame;
    leftFrame->setFrameStyle(QFrame::Panel|QFrame::Raised);
    QComboBox *styleComboBox = new QComboBox;
    styleComboBox->addItems(QStyleFactory::keys());

    QPushButton *button = new QPushButton(tr("pushbutton"));
    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(styleComboBox);
    hbox->addWidget(button);
    
    QSpinBox *spinBox = new QSpinBox;
    spinBox->setRange(0,9);
    QLineEdit *lineEdit = new QLineEdit;
    lineEdit->setText("Hello Hello");
    
    QGroupBox *group = new QGroupBox;
    QRadioButton *radio1 = new QRadioButton(tr("radiobutton1"),group);
    QRadioButton *radio2 = new QRadioButton(tr("radiobutton2"),group);
    QRadioButton *radio3 = new QRadioButton(tr("radiobutton3"),group);
    QCheckBox *checkBox = new QCheckBox(tr("checkbox"),group);
    QVBoxLayout *groupLayout = new QVBoxLayout;
    groupLayout->addWidget(radio1);
    groupLayout->addWidget(radio2);
    groupLayout->addWidget(radio3);
    groupLayout->addWidget(checkBox);
    group->setLayout(groupLayout);
    
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addLayout(hbox);
    vbox->addWidget(spinBox);
    vbox->addWidget(lineEdit);
    vbox->addWidget(group);
    
    leftFrame->setLayout(vbox);

    connect(styleComboBox,SIGNAL(activated(QString)),this,SLOT(slotChangeStyle(QString)));
    slotChangeStyle(QStyleFactory::keys()[0]);
}

void
Style::createRightLayout()
{
    rightFrame = new QFrame;
    rightFrame->setFrameStyle(QFrame::Panel|QFrame::Raised);
    
    QSlider * slider = new QSlider(Qt::Horizontal);
    QTableWidget *table = new QTableWidget;
    table->setColumnCount(3);
    table->setRowCount(4);
    QTextEdit *edit = new QTextEdit;
    edit->setText("Hello World!\n Qt by Trolltech is the standard framework for developing high-performance cross-platform applications.Qt provides single-source portability across Microsoft Windows, Mac OS X, Linux, all major commercial Unix variants, and embedded Linux. On embedded Linux, the Qt API is available as Qtopia Core.");
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(10);
    layout->addWidget(slider);
    layout->addWidget(table);
    layout->addWidget(edit);
    
    rightFrame->setLayout(layout);
}

void
Style::slotChangeStyle(QString style)
{

    QApplication::setStyle(QStyleFactory::create(style));
    QApplication::setPalette(QApplication::style()->standardPalette());
}

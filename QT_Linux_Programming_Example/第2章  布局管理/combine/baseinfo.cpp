#include "baseinfo.h"

#include <QLayout>
#include <QTextCodec>
#include <QPixmap>

BaseInfo::BaseInfo(QWidget *parent, Qt::WindowFlags f1)
        : QWidget(parent,f1)
{
   
    // create 
    label1 = new QLabel(tr("User Name:"));
    label2 = new QLabel(tr("Name:"));
    label3 = new QLabel(tr("Sex:"));
    label4 = new QLabel(tr("Dempartment:"));
    label5 = new QLabel(tr("Age:"));
    labelOther = new QLabel(tr("Other:"));
    labelOther->setFrameStyle(QFrame::Panel|QFrame::Sunken);  
    label6 = new QLabel(tr("Discription:"));
    
    label7 = new QLabel(tr("Icon:"));
    labelIcon = new QLabel();
    QPixmap icon(":/images/icon.png");
    labelIcon->resize(icon.width(),icon.height());
    labelIcon->setPixmap(icon);
    pushButtonIcon = new QPushButton();
    pushButtonIcon->setText(tr("Change"));
    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->setSpacing(20);
    hLayout->addWidget(label7);
    hLayout->addWidget(labelIcon);
    hLayout->addWidget(pushButtonIcon);
    
    lineEditUser = new QLineEdit();
    lineEditName = new QLineEdit();
    
    comboBoxSex = new QComboBox();
    comboBoxSex->insertItem(0,tr("Female"));
    comboBoxSex->insertItem(1,tr("Male"));
    
    textEditDepartment = new QTextEdit();
        
    lineEditAge = new QLineEdit();
    textEditDisc = new QTextEdit();
    
    //  Lay out
    // left layout -- is a grid layout
    QGridLayout * leftLayout = new QGridLayout();

    int labelCol = 0;
    int contentCol = 1;
    
    leftLayout->addWidget(label1,0,labelCol);		// user name row
    leftLayout->addWidget(lineEditUser,0,contentCol);
    
    leftLayout->addWidget(label2,1,labelCol);		// name row
    leftLayout->addWidget(lineEditName,1,contentCol);
    
    leftLayout->addWidget(label3,2,labelCol);		// sex row
    leftLayout->addWidget(comboBoxSex,2,contentCol);
    
    leftLayout->addWidget(label4,3,labelCol,Qt::AlignTop);  // department row
    leftLayout->addWidget(textEditDepartment,3,contentCol);

    leftLayout->addWidget(label5,4,labelCol);		// age row
    leftLayout->addWidget(lineEditAge,4,contentCol);
    
    leftLayout->addWidget(labelOther,5,labelCol,1,2);		// other

    leftLayout->setColumnStretch(0,1);
    leftLayout->setColumnStretch(1,3);
    
    leftLayout->setRowStretch(0,1);
    leftLayout->setRowStretch(3,3);
    
  
    // right layout -- is a vBoxLayout
    QVBoxLayout *rightLayout = new QVBoxLayout();
    rightLayout->setMargin(10);
    rightLayout->addLayout(hLayout);
    rightLayout->addWidget(label6);
    rightLayout->addWidget(textEditDisc);
    

    // main layout -- is a hBoxLayout
    QHBoxLayout * mainLayout = new QHBoxLayout(this);
    mainLayout->setMargin(15);
    mainLayout->setSpacing(10);
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);
    
}


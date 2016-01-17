#include "inputdialog.h"

InputDlg::InputDlg()
{

    setWindowTitle(tr("Input Dialog"));

    label1 = new QLabel(tr("Name : "));
    label2 = new QLabel(tr("Sex : "));
    label3 = new QLabel(tr("Age : "));
    label4 = new QLabel(tr("Stature : "));
    
    nameLabel = new QLabel(tr("LiMing"));
    nameLabel->setFrameStyle(QFrame::Panel|QFrame::Sunken);
    sexLabel = new QLabel(tr("male"));
    sexLabel->setFrameStyle(QFrame::Panel|QFrame::Sunken);
    ageLabel = new QLabel(tr("25"));
    ageLabel->setFrameStyle(QFrame::Panel|QFrame::Sunken);
    statureLabel = new QLabel("175.5");
    statureLabel->setFrameStyle(QFrame::Panel|QFrame::Sunken);
    
    nameButton = new QPushButton;
    nameButton->setIcon(QIcon(":/images/btn.png"));
    sexButton = new QPushButton;
    sexButton->setIcon(QIcon(":/images/btn.png"));
    ageButton = new QPushButton;
    ageButton->setIcon(QIcon(":/images/btn.png"));
    statureButton = new QPushButton;
    statureButton->setIcon(QIcon(":/images/btn.png"));
    

    QGridLayout *layout = new QGridLayout( this );
    int name = 0;
    int sex = 1;
    int age = 2;
    int stature = 3;
    layout->addWidget( label1, name, 0 );	
    layout->addWidget( nameLabel, name, 1 );
    layout->addWidget( nameButton, name, 2 );
    layout->addWidget( label2, sex, 0 );
    layout->addWidget( sexLabel, sex, 1 );
    layout->addWidget( sexButton, sex, 2 );
    layout->addWidget( label3, age, 0 );
    layout->addWidget( ageLabel, age, 1 );
    layout->addWidget( ageButton, age, 2 );
    layout->addWidget( label4, stature, 0 );
    layout->addWidget( statureLabel, stature, 1 );
    layout->addWidget( statureButton, stature, 2 );
    layout->setMargin(15);
    layout->setSpacing(10);
    layout->setColumnMinimumWidth(1,120);
    
    connect(nameButton,SIGNAL(clicked()),this,SLOT(slotName()));
    connect(sexButton,SIGNAL(clicked()),this,SLOT(slotSex()));
    connect(ageButton,SIGNAL(clicked()),this,SLOT(slotAge()));
    connect(statureButton,SIGNAL(clicked()),this,SLOT(slotStature()));
}

void InputDlg::slotName()
{                              
    bool ok;
    QString name = QInputDialog::getText(this,tr("User Name"),
    			tr("Please input new name:"),QLineEdit::Normal,nameLabel->text(),&ok);  
    if(ok && !name.isEmpty())
    	nameLabel->setText(name);
}                                                           
                                                            
void InputDlg::slotSex()                    
{             
    QStringList list;
    list << tr("male") << tr("female");
    bool ok;
    QString sex = QInputDialog::getItem(this,tr("Sex"),
    			tr("Please select sex:"),list,0,false,&ok);
    if (ok)
    	sexLabel->setText(sex);
}                                                           
                                                            
void InputDlg::slotAge()                     
{                             
    bool ok;                  
    int age = QInputDialog::getInteger(this,tr("User Age"),
    			tr("Please input age:"),ageLabel->text().toInt(),0,150,1,&ok);  
    if(ok)
      	ageLabel->setText(QString(tr("%1")).arg(age));
}

void InputDlg::slotStature()                     
{                             
    bool ok;
    double d = QInputDialog::getDouble(this,tr("Stature"),
    			tr("Please input stature:"),175.00,0,230.00,1,&ok);  
    if(ok)
    	statureLabel->setText(QString(tr("%1")).arg(d));
}

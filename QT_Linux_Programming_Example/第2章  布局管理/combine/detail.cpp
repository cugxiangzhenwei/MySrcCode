#include <QTextCodec>
#include <QLayout>
#include "detail.h"

Detail::Detail(QWidget *parent, Qt::WindowFlags f1)
	: QWidget(parent,f1)
{
    label1 = new QLabel(tr("Country/Zone:"));
    label2 = new QLabel(tr("Province:"));
    label3 = new QLabel(tr("City:"));
    label4 = new QLabel(tr("Discription:"));
    
    comboBoxCountry = new QComboBox();
    comboBoxCountry->insertItem(0,tr("China"));
    
    comboBoxProvince = new QComboBox();
    comboBoxProvince->insertItem(0,tr("JiangSu"));
    
    lineeditCity = new QLineEdit();
    texteditPriv = new QTextEdit();
    
    QGridLayout *mainLayout = new QGridLayout(this);
    int labelCol = 0;
    int contCol = 1;
    mainLayout->addWidget(label1,0,labelCol);
    mainLayout->addWidget(comboBoxCountry,0,contCol);
    mainLayout->addWidget(label2,1,labelCol);
    mainLayout->addWidget(comboBoxProvince,1,contCol);
    mainLayout->addWidget(label3,2,labelCol);
    mainLayout->addWidget(lineeditCity,2,contCol);
    mainLayout->addWidget(label4,3,labelCol,Qt::AlignTop);
    mainLayout->addWidget(texteditPriv,3,contCol);
    mainLayout->setColumnStretch(0,10);
    mainLayout->setColumnStretch(1,30);

    mainLayout->setMargin(20);
    mainLayout->setSpacing(25);
}

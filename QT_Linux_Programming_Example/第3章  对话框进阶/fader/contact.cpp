#include "contact.h"

//#include <qvariant.h>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QLayout>
#include <QToolTip>
#include <QWhatsThis>
#include <QLayout>
#include <QTextCodec>

Contact::Contact( QWidget* parent, Qt::WindowFlags fl )
    : QWidget( parent, fl )
{	
    textLabel1 = new QLabel( tr("email:") );
    textLabel2 = new QLabel( tr("address:") );
    textLabel3 = new QLabel( tr("zip code:") );
    textLabel4 = new QLabel( tr("mobile:") );
    textLabel5 = new QLabel( tr("office:") );

    lineEditEmail = new QLineEdit();
    lineEditAddress = new QLineEdit();
    checkBoxMsg = new QCheckBox(tr("receive msg"));
    lineEditMobile = new QLineEdit();
    lineEditPhone = new QLineEdit();
    lineEditZip = new QLineEdit();
    
    QGridLayout *grid = new QGridLayout(this);
    grid->setMargin(20);
    grid->setSpacing(10);
    int labelCol = 0;
    int contCol = 1;
    int otherCol = 2;
    grid->addWidget(textLabel1,0,labelCol);
    grid->addWidget(lineEditEmail,0,contCol);
    grid->addWidget(textLabel2,1,labelCol);
    grid->addWidget(lineEditAddress,1,contCol);
    grid->addWidget(textLabel3,2,labelCol);
    grid->addWidget(lineEditZip,2,contCol);
    grid->addWidget(textLabel4,3,labelCol);
    grid->addWidget(lineEditMobile,3,contCol);
    grid->addWidget(checkBoxMsg,3,otherCol);
    grid->addWidget(textLabel5,4,labelCol);
    grid->addWidget(lineEditPhone,4,contCol);
    
    grid->setColumnStretch(0,10);
    grid->setColumnStretch(1,30);
    grid->setColumnStretch(2,10);

}

/*
 *  Destroys the object and frees any allocated resources
 */
Contact::~Contact()
{
    // no need to delete child widgets, Qt does it all for us
}


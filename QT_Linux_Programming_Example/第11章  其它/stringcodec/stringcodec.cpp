#include "stringcodec.h"

StringCodec::StringCodec( QWidget *parent, Qt::WindowFlags  f )
    : QDialog( parent, f )
{
    QFont font("ZYSong18030",12);
    setFont(font);	

    setWindowTitle(tr("String codec transfer"));

    layout = new QGridLayout( this );  

    labelSource=new QLabel(tr("Source string:"));
    labelUnicode=new QLabel(tr("Unicode:"));
    labelBigEndianUnicode=new QLabel(tr("BigEndianUnicode:"));
    labelUtf8=new QLabel(tr("Utf8:"));
    labelLocal=new QLabel(tr("Local:"));
    
    lenUnicode=new QLabel(tr("Unicode Len:"));
    lenBigEndianUnicode=new QLabel(tr("BigEndianUnicode Len:"));
    lenUtf8=new QLabel(tr("Utf8 Len:"));
    lenLocal=new QLabel(tr("Local Len:"));

    lineEditSource = new QLineEdit();
    lineEditUnicode = new QLineEdit();
    lineEditBigEndianUnicode = new QLineEdit();
    lineEditUtf8 = new QLineEdit( );
    lineEditLocal = new QLineEdit( );    

    lineEditUnicodeLen = new QLineEdit();
    lineEditBigEndianUnicodeLen = new QLineEdit();
    lineEditUtf8Len = new QLineEdit( );
    lineEditLocalLen = new QLineEdit( ); 

    pushButtonStart = new QPushButton();
    pushButtonStart->setText(tr("Start"));

    layout->addWidget( labelSource, 0, 0 );	
    layout->addWidget( lineEditSource, 0, 1, 1, 3);
    
    layout->addWidget( labelUnicode, 1, 0 );
    layout->addWidget( lineEditUnicode, 1, 1 );
    layout->addWidget( lenUnicode, 1, 2 );
    layout->addWidget( lineEditUnicodeLen, 1, 3 );

    layout->addWidget( labelBigEndianUnicode, 2, 0 );
    layout->addWidget( lineEditBigEndianUnicode, 2, 1 );
    layout->addWidget( lenBigEndianUnicode, 2, 2 );
    layout->addWidget( lineEditBigEndianUnicodeLen, 2, 3 );
    
    layout->addWidget( labelUtf8, 3, 0 );
    layout->addWidget( lineEditUtf8, 3, 1 );
    layout->addWidget( lenUtf8, 3, 2 );
    layout->addWidget( lineEditUtf8Len, 3, 3 );
    
    layout->addWidget( labelLocal, 4, 0 );
    layout->addWidget( lineEditLocal, 4, 1 );
    layout->addWidget( lenLocal, 4, 2 );
    layout->addWidget( lineEditLocalLen, 4, 3 );
   
   	layout->addWidget( pushButtonStart, 5, 3);  

   
    layout->setMargin(15);
    layout->setSpacing(10);
    
    connect(pushButtonStart,SIGNAL(clicked()),this,SLOT(slotStart()));
}

StringCodec::~StringCodec()
{
}
                              
    
                                                            
void StringCodec::slotStart()                     
{                
	lineEditUnicode->clear();
	lineEditBigEndianUnicode->clear();
	lineEditUtf8->clear();
	lineEditLocal->clear();
	
	QString source=lineEditSource->text();

	const QChar *u=source.unicode();
	
	//Get Unicode
	lineEditUnicodeLen->setText(QString::number(source.length()*2));
	for(int i=0;i<source.length();i++)
	{
		const ushort unicode=u[i].unicode();

		lineEditUnicode->insert(QString::number(unicode%256,16)+" ");
		lineEditUnicode->insert(QString::number(unicode/256,16)+" ");
	}

	//Get BigEndian Unicode
	lineEditBigEndianUnicodeLen->setText(QString::number(source.length()*2));
	for(int i=0;i<source.length();i++)
	{
		const ushort unicode=u[i].unicode();
		lineEditBigEndianUnicode->insert(QString::number(unicode/256,16)+" ");
		lineEditBigEndianUnicode->insert(QString::number(unicode%256,16)+" ");
	}

	//Get Utf8
	QByteArray b=source.toUtf8(); 
	lineEditUtf8Len->setText(QString::number(b.length()));
	for(int i=0;i<b.length();i++)
	{
		const unsigned char a=b[i];
		lineEditUtf8->insert(QString::number(a,16)+" ");
	}
	
	//Get Local
	QByteArray ba=source.toLocal8Bit (); 
	lineEditLocalLen->setText(QString::number(ba.length()));
	for(int i=0;i<ba.length();i++)
	{
		const unsigned char a=ba[i];
		lineEditLocal->insert(QString::number(a,16)+" ");
	}
	
}

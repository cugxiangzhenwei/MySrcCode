#include "tcpserver.h"

TcpServer::TcpServer( QWidget *parent, Qt::WindowFlags  f )
    : QDialog( parent, f )
{  
    QFont font("ZYSong18030",12);
    setFont(font);
    	
    setWindowTitle(tr("TCP Server"));

    QVBoxLayout *vbMain = new QVBoxLayout( this );

 	ListWidgetContent = new QListWidget( this); 
    vbMain->addWidget( ListWidgetContent );    

    QHBoxLayout *hb = new QHBoxLayout( );
    
    LabelPort = new QLabel( this );
    LabelPort->setText(tr("Port:"));
    hb->addWidget( LabelPort );
   
    LineEditPort = new QLineEdit(this);
    hb->addWidget( LineEditPort );
    
    vbMain->addLayout(hb);
           
    PushButtonCreate = new QPushButton( this);
    PushButtonCreate->setText( tr( "Create" ) );  
    vbMain->addWidget( PushButtonCreate );

    connect(PushButtonCreate,SIGNAL(clicked()),this,SLOT(slotCreateServer()));	

	port = 8010; 
	LineEditPort->setText(QString::number(port)); 

}

TcpServer::~TcpServer()
{
}

void TcpServer::slotCreateServer()                     
{         
	server = new Server(this,port); 
	connect(server,SIGNAL(updateServer(QString,int)),this,SLOT(updateServer(QString,int)));

	PushButtonCreate->setEnabled(false);
}

void TcpServer::updateServer(QString msg,int length)
{
	ListWidgetContent->addItem (msg.left(length) );
}
#include "tcpclient.h"

TcpClient::TcpClient( QWidget *parent, Qt::WindowFlags  f )
    : QDialog( parent, f )
{  
    QFont font("ZYSong18030",12);
    setFont(font);
    	
    setWindowTitle(tr("TCP Client"));

    QVBoxLayout *vbMain = new QVBoxLayout( this );
    
 	ListWidgetContent = new QListWidget( this); 
    vbMain->addWidget( ListWidgetContent ); 
    
	QHBoxLayout *hb1 = new QHBoxLayout( );
	  
    LineEditSend = new QLineEdit(this);
    hb1->addWidget( LineEditSend );	

    PushButtonSend = new QPushButton( this);
    PushButtonSend->setText( tr( "Send" ) );  
    hb1->addWidget( PushButtonSend );

	vbMain->addLayout( hb1 );

    QHBoxLayout *hb2 = new QHBoxLayout( );

    LabelUser = new QLabel( this );
    LabelUser->setText(tr("User Name:"));
    hb2->addWidget( LabelUser );
   
    LineEditUser = new QLineEdit(this);
    hb2->addWidget( LineEditUser );

	QHBoxLayout *hb3 = new QHBoxLayout( );

    LabelServerIP = new QLabel( this );
    LabelServerIP->setText(tr("Server:"));
    hb3->addWidget( LabelServerIP );
   
    LineEditServerIP = new QLineEdit(this);
    hb3->addWidget( LineEditServerIP );
    
    QHBoxLayout *hb4 = new QHBoxLayout( );
    
    LabelPort = new QLabel( this );
    LabelPort->setText(tr("Port:"));
    hb4->addWidget( LabelPort );
   
    LineEditPort = new QLineEdit(this);
    hb4->addWidget( LineEditPort );
    
    vbMain->addLayout(hb2);
    vbMain->addLayout(hb3);
    vbMain->addLayout(hb4);
           
    PushButtonEnter = new QPushButton( this);
    PushButtonEnter->setText( tr( "Enter" ) );  
    vbMain->addWidget( PushButtonEnter );

    connect(PushButtonEnter,SIGNAL(clicked()),this,SLOT(slotEnter()));
    connect(PushButtonSend,SIGNAL(clicked()),this,SLOT(slotSend()));	

	serverIP = new QHostAddress();
	port = 8010; 
	LineEditPort->setText(QString::number(port)); 
	
	status=false;
	
	PushButtonSend->setEnabled( false ); 
}

TcpClient::~TcpClient()
{
}
                                                                             
void TcpClient::slotEnter()                     
{         
	if(!status)
	{
		QString ip=LineEditServerIP->text();
		if(!serverIP->setAddress(ip))
		{
			QMessageBox::information(this,tr("error"),tr("server ip address error!"));
			return;
		}
		if(LineEditUser->text()=="")
		{
			QMessageBox::information(this,tr("error"),tr("User name error!"));
			return ;
		}	
		userName=LineEditUser->text();
	
		tcpSocket = new QTcpSocket(this);
		connect(tcpSocket,SIGNAL(connected()),this,SLOT(slotConnected()));
		connect(tcpSocket,SIGNAL(disconnected()),this,SLOT(slotDisconnected()));
		connect(tcpSocket, SIGNAL(readyRead()),this, SLOT(dataReceived()));
		
		tcpSocket->connectToHost ( *serverIP, port);
		
		status=true;
	}
	else
	{
		int length = 0; 
		
		QString msg=userName+tr(":Leave Chat Room");
	    if((length=tcpSocket->write(msg.toLatin1(),msg.length()))!=msg.length())
	    {
	    	return ;
	    }			
		tcpSocket->disconnectFromHost();
		
		status=false;
	}
	
}

void TcpClient::slotConnected()                     
{
	int length = 0; 
	PushButtonSend->setEnabled( true );
	PushButtonEnter->setText(tr("Leave"));
	
	QString msg=userName+tr(":Enter Chat Room");
    if((length=tcpSocket->write(msg.toLatin1(),msg.length()))!=msg.length())
    {
    	return ;
    }	
}

void TcpClient::slotDisconnected()                     
{
	PushButtonSend->setEnabled( false );
	PushButtonEnter->setText(tr("Enter"));
}

void TcpClient::slotSend()                     
{         
	if(LineEditSend->text()=="")
	{
		return ;
	}
	
	QString msg=userName+":"+LineEditSend->text();
	
	tcpSocket->write(msg.toLatin1(),msg.length());
	LineEditSend->clear();
}

void TcpClient::dataReceived()
{
    while (tcpSocket->bytesAvailable()>0) 
    {
 		QByteArray datagram;
 		datagram.resize(tcpSocket->bytesAvailable());
 		QHostAddress sender;


 		tcpSocket->read(datagram.data(), datagram.size());

		QString msg=datagram.data();
	
		ListWidgetContent->addItem (msg.left(datagram.size()));
    }

}
#include "udpserver.h"

UdpServer::UdpServer( QWidget *parent, Qt::WindowFlags  f )
    : QDialog( parent, f )
{  
    setWindowTitle(tr("UDP Server"));

    QVBoxLayout *vbMain = new QVBoxLayout( this );

    LabelTimer = new QLabel( this );
    LabelTimer->setText(tr("Timer:"));
    vbMain->addWidget( LabelTimer );
   
    LineEditText = new QLineEdit(this);
    vbMain->addWidget( LineEditText );
           
    PushButtonStart = new QPushButton( this);
    PushButtonStart->setText( tr( "Start" ) );  
    vbMain->addWidget( PushButtonStart );

    connect(PushButtonStart,SIGNAL(clicked()),this,SLOT(PushButtonStart_clicked()));	

    port = 5555;

	isStarted=false;
	
	udpSocket = new QUdpSocket(this); 
	
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timeout()));
}

UdpServer::~UdpServer()
{
}
                                                                             
void UdpServer::PushButtonStart_clicked()                     
{          
	if(!isStarted)
	{      	      
	    timer -> start(1000);
	    isStarted = true;
	    PushButtonStart->setText(tr("Stop"));
	}
	else
	{
		timer -> stop();
		isStarted = false;
		PushButtonStart->setText(tr("Start"));
	}
}

void UdpServer::timeout()                     
{                          
	QString msg = LineEditText->text();
	int length = 0;
	if(msg == "")
	{
		return;
	}
    if((length=udpSocket->writeDatagram(msg.toLatin1(),msg.length(), QHostAddress::Broadcast, port))!=msg.length())
    {
    	return ;
    }	
}

#include "udpclient.h"

UdpClient::UdpClient( QWidget *parent, Qt::WindowFlags  f )
    : QDialog( parent, f )
{  
    setWindowTitle(tr("UDP Client"));

    QVBoxLayout *vbMain = new QVBoxLayout( this );

 	TextEditReceive = new QTextEdit( this); 
    vbMain->addWidget( TextEditReceive );    
       
    PushButtonClose = new QPushButton( this);
    PushButtonClose->setText( tr( "Close" ) );  
    vbMain->addWidget( PushButtonClose );

    connect(PushButtonClose,SIGNAL(clicked()),this,SLOT(PushButtonClose_clicked()));	
	port = 5555;

	udpSocket = new QUdpSocket(this); 
	connect(udpSocket, SIGNAL(readyRead()),this, SLOT(dataReceived()));
	
	bool result=udpSocket->bind(port);
	if(!result)
	{
		QMessageBox::information(this,tr("error"),tr("udp socket create error!"));
		return;		
	}	
}

UdpClient::~UdpClient()
{
}
                                                                             
void UdpClient::PushButtonClose_clicked()                     
{          
	accept();
}

void UdpClient::dataReceived()
{
    while (udpSocket->hasPendingDatagrams()) 
    {
	 	QByteArray datagram;
	 	datagram.resize(udpSocket->pendingDatagramSize());

	 	udpSocket->readDatagram(datagram.data(), datagram.size());
	
		QString msg=datagram.data();	
		TextEditReceive->insertPlainText(msg);
    }
}
#include "tcpserver.h"

TcpClientSocket::TcpClientSocket( QObject *parent)
{  
    connect(this, SIGNAL(readyRead()),this, SLOT(dataReceived()));	 
    connect(this, SIGNAL(disconnected()),this, SLOT(slotDisconnected()));
}

TcpClientSocket::~TcpClientSocket()
{
}
                                                                             
void TcpClientSocket::dataReceived()
{
    while (bytesAvailable()>0) 
    {
 		char buf[1024];
 		int length=bytesAvailable();
 		read(buf, length);

		QString msg=buf;
		
		emit updateClients(msg,length);
    }	
}

void TcpClientSocket::slotDisconnected()
{
	emit disconnected(this->socketDescriptor ());
}
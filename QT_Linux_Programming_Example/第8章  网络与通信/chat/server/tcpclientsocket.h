#ifndef TCPCLIENTSOCKET_H
#define TCPCLIENTSOCKET_H

#include <QtGui>
#include <QtNetwork>

class TcpClientSocket : public QTcpSocket
{
    Q_OBJECT
public:
    TcpClientSocket( QObject *parent=0);
    ~TcpClientSocket();
signals:
	void updateClients(QString,int);
	void disconnected(int);
protected slots: 
    void dataReceived();    
    void slotDisconnected();
};


#endif 

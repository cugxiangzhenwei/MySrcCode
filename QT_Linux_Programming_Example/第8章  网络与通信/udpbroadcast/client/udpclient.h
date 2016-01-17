#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include <QtGui>
#include <QtNetwork>

class UdpClient : public QDialog
{
    Q_OBJECT
public:
    UdpClient( QWidget *parent=0, Qt::WindowFlags  f=0 );
    ~UdpClient();
public:
	QTextEdit* TextEditReceive;    
    QPushButton* PushButtonClose;

    QUdpSocket *udpSocket;
    quint16 port;

public slots:
    void PushButtonClose_clicked();
    void dataReceived();
};


#endif

#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QtGui>
#include <QtNetwork>

class TcpClient : public QDialog
{
    Q_OBJECT
public:
    TcpClient( QWidget *parent=0, Qt::WindowFlags  f=0 );
    ~TcpClient();
public:
	QListWidget *ListWidgetContent;
	QLineEdit* LineEditSend;
	QPushButton* PushButtonSend;
    QLabel* LabelUser;
    QLineEdit* LineEditUser;
    QLabel* LabelServerIP;
    QLineEdit* LineEditServerIP;    	
    QLabel* LabelPort;
    QLineEdit* LineEditPort;
    QPushButton* PushButtonEnter;
    
    QString userName;
    int port;
    QHostAddress *serverIP;
	QTcpSocket *tcpSocket;
	
	bool status;
public slots:
    void slotEnter();
    void slotConnected();
    void slotDisconnected();
    void slotSend();
    void dataReceived();
};


#endif 

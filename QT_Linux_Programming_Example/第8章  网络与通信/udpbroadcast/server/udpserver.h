#ifndef UDPSERVER_H
#define UDPSERVER_H

#include <QtGui>
#include <QtNetwork>
#include <QtDebug>

class UdpServer : public QDialog
{
    Q_OBJECT
public:
    UdpServer( QWidget *parent=0, Qt::WindowFlags  f=0 );
    ~UdpServer();
public:
    QLabel* LabelTimer;
    QLineEdit* LineEditText;
    QPushButton* PushButtonStart;

    QUdpSocket *udpSocket;
    quint16 port;
    bool isStarted;
    
    QTimer *timer;

public slots:
    void PushButtonStart_clicked();
	void timeout();
};


#endif 

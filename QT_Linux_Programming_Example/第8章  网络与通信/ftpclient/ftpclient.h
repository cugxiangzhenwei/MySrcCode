#ifndef FTPCLIENT_H
#define FTPCLIENT_H

#include <QtGui>
#include <QtNetwork>

class FtpClient : public QDialog
{
    Q_OBJECT
public:
    FtpClient( QWidget *parent=0, Qt::WindowFlags  f=0 );
    ~FtpClient();
public:
	QLabel* LabelServer;
	QLineEdit* LineEditServer;
	QLabel* LabelUser;
	QLineEdit* LineEditUser;
	QLabel* LabelPassword;
	QLineEdit* LineEditPassword;
    QPushButton* PushButtonLogin;
    QPushButton* PushButtonPut;
    QPushButton* PushButtonGet;

    enum STATUS{None,LOGIN,PUT,GET};
    STATUS curStatus;
    
    QFtp *ftpClient;
	QFile *localFile;
	
public slots:
    void slotLogin();
    void slotPut();
    void slotGet();
	void slotStateChanged ( int state );
	void slotDone ( bool error );
};


#endif

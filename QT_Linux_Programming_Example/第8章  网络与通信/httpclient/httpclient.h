#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <QtGui>
#include <QtNetwork>

class HttpClient : public QDialog
{
    Q_OBJECT
public:
    HttpClient( QWidget *parent=0, Qt::WindowFlags  f=0 );
    ~HttpClient();
public:
	QLabel* urlLabel;
	QLineEdit* urlLineEdit;
	QLabel* LabelUser;
	QProgressBar* progressBar;
    QPushButton* downloadPushButton;
    QPushButton* cancelPushButton;
    QPushButton* exitPushButton;

    QHttp* httpClient;
    QFile* file;
    
    int requestId;
	bool httpRequestAborted;
public slots:
    void slotDownload();
    void slotCancel();
    void slotExit();
    void httpRequestFinished(int, bool);
    void httpDataReadProgress(int, int);
    void httpResponseHeaderReceived(const QHttpResponseHeader &);
};


#endif

#include "httpclient.h"

HttpClient::HttpClient( QWidget *parent, Qt::WindowFlags  f )
    : QDialog( parent, f )
{  
    QFont font("ZYSong18030",12);
    setFont(font);	
    
    setWindowTitle(tr("Http Client"));
    
    urlLabel = new QLabel( this );
    urlLabel->setText(tr("Url:"));
    
    urlLineEdit = new QLineEdit( this );
    
    progressBar = new QProgressBar( this );
    
    downloadPushButton = new QPushButton( this);
    downloadPushButton->setText( tr( "Download" ) );  
    
    cancelPushButton = new QPushButton( this);
    cancelPushButton->setText( tr( "Cancel" ) );      
    
    exitPushButton = new QPushButton( this);
    exitPushButton->setText( tr( "Exit" ) );      
    
    QHBoxLayout *hbLayout1 = new QHBoxLayout();
    hbLayout1->addWidget( urlLabel );
    hbLayout1->addWidget( urlLineEdit );

    QHBoxLayout *hbLayout2 = new QHBoxLayout();
    hbLayout2->addWidget( progressBar );
    
    QHBoxLayout *hbLayout3 = new QHBoxLayout();
    hbLayout3->addWidget( downloadPushButton );
    hbLayout3->addWidget( cancelPushButton ); 
    hbLayout3->addWidget( exitPushButton );    
    
    QVBoxLayout *vbLayout = new QVBoxLayout( this );
    vbLayout->addLayout( hbLayout1 );
    vbLayout->addLayout( hbLayout2 );
    vbLayout->addLayout( hbLayout3 );

    httpClient = new QHttp(this);

    connect(httpClient, SIGNAL(requestFinished(int, bool)),this, SLOT(httpRequestFinished(int, bool)));
    connect(httpClient, SIGNAL(dataReadProgress(int, int)),this, SLOT(httpDataReadProgress(int, int)));
    connect(httpClient, SIGNAL(responseHeaderReceived(const QHttpResponseHeader &)),this, SLOT(httpResponseHeaderReceived(const QHttpResponseHeader &)));


	connect(downloadPushButton,SIGNAL(clicked()),this,SLOT(slotDownload()));
    connect(cancelPushButton,SIGNAL(clicked()),this,SLOT(slotCancel()));
    connect(exitPushButton,SIGNAL(clicked()),this,SLOT(slotExit()));
    
	cancelPushButton->setEnabled(false);

}

HttpClient::~HttpClient()
{
}
                                                                             
void HttpClient::slotDownload()
{
	QUrl url(urlLineEdit->text());
    QFileInfo fileInfo(url.path());
    QString fileName = fileInfo.fileName();
	fileName = "D:\\TEST_" + fileName; 

    if (QFile::exists(fileName)) 
    {
        QMessageBox::information(this, tr("Error"),tr("File already exists!"));
        return;
    }

    file = new QFile(fileName);
    if (!file->open(QIODevice::WriteOnly)) 
    {
        QMessageBox::information(this, tr("Error"),tr("Save file error!"));
        delete file;
        file = 0;
        return;
    }
    
    httpClient->setHost(url.host(), url.port() != -1 ? url.port() : 80);
    if (!url.userName().isEmpty())
    {
        httpClient->setUser(url.userName(), url.password());
    }

    httpRequestAborted = false;
    requestId = httpClient->get(url.path(), file);

    progressBar->reset();

    downloadPushButton->setEnabled(false);
    cancelPushButton->setEnabled(true);
}

void HttpClient::slotCancel()
{
    httpRequestAborted = true;
    httpClient->abort();
    downloadPushButton->setEnabled(true);
    cancelPushButton->setEnabled(false);	
}
	
void HttpClient::slotExit()
{
	accept();
}

void HttpClient::httpRequestFinished(int id, bool error)
{
    if (httpRequestAborted) 
    {
        if (file) 
        {
            file->close();
            file->remove();
            delete file;
            file = 0;
        }

        progressBar->reset();
        return;
    }

    if (id == requestId)
    {
		progressBar->reset();
		file->close();
		
		if (error) 
		{
		    file->remove();
		    QMessageBox::information(this, tr("Error"),tr("Download failed!"));
		} 

		downloadPushButton->setEnabled(true);
		cancelPushButton->setEnabled(false);
		delete file;
		file = 0;
	}
}

void HttpClient::httpDataReadProgress(int done, int total)
{
	progressBar->setMaximum(total);
    progressBar->setValue(done);
}

void HttpClient::httpResponseHeaderReceived(const QHttpResponseHeader &responseHeader)
{
	if (responseHeader.statusCode() != 200) 
	{
		QMessageBox::information(this, tr("Error"),	tr("Download error!"));
		httpRequestAborted = true;
		progressBar->reset();
		httpClient->abort();
		return;
	}
}
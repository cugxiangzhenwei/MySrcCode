#include "ftpclient.h"

FtpClient::FtpClient( QWidget *parent, Qt::WindowFlags  f )
    : QDialog( parent, f )
{  
    setWindowTitle(tr("FTP Client"));
    
    LabelServer = new QLabel( this );
    LabelServer->setText(tr("Server Address:"));
    
    LineEditServer = new QLineEdit( this );
    
    LabelUser = new QLabel( this );
    LabelUser->setText(tr("User Name:"));
    
    LineEditUser = new QLineEdit( this );
    
    LabelPassword = new QLabel( this );
    LabelPassword->setText(tr("Password:"));
    
    LineEditPassword = new QLineEdit( this );
        
    PushButtonLogin = new QPushButton( this);
    PushButtonLogin->setText( tr( "Login" ) );  
    
    PushButtonPut = new QPushButton( this);
    PushButtonPut->setText( tr( "Put" ) );      
    
    PushButtonGet = new QPushButton( this);
    PushButtonGet->setText( tr( "Get" ) );      
    
    QGridLayout *gLayout = new QGridLayout();
    gLayout->addWidget( LabelServer,0,0 );
    gLayout->addWidget( LineEditServer,0,1 );
    gLayout->addWidget( LabelUser,1,0 );
    gLayout->addWidget( LineEditUser,1,1 );
    gLayout->addWidget( LabelPassword,2,0 );
    gLayout->addWidget( LineEditPassword,2,1 );
    
    QHBoxLayout *hbLayout = new QHBoxLayout();
    hbLayout->addWidget( PushButtonPut );
    hbLayout->addWidget( PushButtonGet );
    
    QVBoxLayout *vbLayout = new QVBoxLayout( this );
    vbLayout->addLayout( gLayout );
    vbLayout->addWidget( PushButtonLogin );
    vbLayout->addLayout( hbLayout );

	connect(PushButtonLogin,SIGNAL(clicked()),this,SLOT(slotLogin()));
    connect(PushButtonPut,SIGNAL(clicked()),this,SLOT(slotPut()));
    connect(PushButtonGet,SIGNAL(clicked()),this,SLOT(slotGet()));
    
	PushButtonPut->setEnabled(false);
	PushButtonGet->setEnabled(false);
	
	curStatus=None;
}

FtpClient::~FtpClient()
{
}
                                                                             
void FtpClient::slotLogin()                     
{          
	QString serverAddress = LineEditServer->text();
	if(serverAddress.isEmpty())
	{
		QMessageBox::warning(this,tr("error"),tr("Please input server address!"));
		return;
	}
	QString userName = LineEditUser->text();
	if(userName.isEmpty())
	{
		QMessageBox::warning(this,tr("error"),tr("Please input user name!"));
		return;
	}	
	QString password = LineEditPassword->text();
	
	ftpClient=new QFtp();
	ftpClient->connectToHost(serverAddress);
	
	connect(ftpClient, SIGNAL(stateChanged ( int )), this, SLOT(slotStateChanged ( int ) ) );
	connect(ftpClient, SIGNAL(done ( bool )), this, SLOT(slotDone ( bool ) ) );
	
	ftpClient->login(userName,password);
	curStatus=LOGIN;  
}

void FtpClient::slotPut()                     
{          
	bool ok;
	QString fileName = QInputDialog::getText(this, tr("Put File:"),
		tr("Please input file name:"), QLineEdit::Normal, QString(), &ok);  
	if(ok && !fileName.isEmpty())
	{
		QFile *remoteFileName=new QFile(fileName);
		ftpClient->put(remoteFileName, fileName);
	}
	curStatus=PUT;
}

void FtpClient::slotGet()
{        
	bool ok;
	QString fileName = QInputDialog::getText(this, tr("Get File:"),
		tr("Please input file name:"), QLineEdit::Normal, QString(), &ok);  
	if(ok && !fileName.isEmpty())
	{
		localFile=new QFile(fileName);	
		localFile->open(QIODevice::WriteOnly);
		ftpClient->get(fileName,localFile);
	}	
	curStatus=GET;  
}

void FtpClient::slotStateChanged ( int state )
{
	if(state == QFtp::LoggedIn)
	{
		PushButtonPut->setEnabled(true);
		PushButtonGet->setEnabled(true);
	}
}

void FtpClient::slotDone(bool error)
{
	if(error)
	{
		QMessageBox::warning(this,tr("error"),ftpClient->errorString());
		return;
	}
	if(curStatus == LOGIN)
	{
		PushButtonLogin->setEnabled(false);
		curStatus=None;
	}	
	if(curStatus == PUT)
	{
		QMessageBox::information(this,tr("succeed"),tr("Put file succeed!"));
		curStatus=None;
	}
	if(curStatus == GET)
	{
		localFile->close();
		QMessageBox::information(this,tr("succeed"),tr("Get file succeed!"));
		curStatus=None;
	}
}
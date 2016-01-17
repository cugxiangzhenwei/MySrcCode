#include "fileinformation.h"

FileInformation::FileInformation( QWidget *parent, Qt::WindowFlags  f )
    : QDialog( parent, f )
{  
    setWindowTitle(tr("File Information"));
       
    LabelFileName = new QLabel();
    LabelFileName->setText(tr("File Name:"));
    
    LineEditFileName = new QLineEdit();   

    PushButtonFile = new QPushButton( this);
    PushButtonFile->setText( tr( "File" ) );    

    LabelSize = new QLabel();
    LabelSize->setText(tr("Size:"));
    
    LineEditSize = new QLineEdit();  

    LabelCreated = new QLabel();
    LabelCreated->setText(tr("Created:"));
    
    LineEditCreated = new QLineEdit();
    
    LabelLastModified = new QLabel();
    LabelLastModified->setText(tr("Last Modified:"));
    
    LineEditLastModified = new QLineEdit();
    
    LabelLastRead = new QLabel();
    LabelLastRead->setText(tr("Last Read:"));
    
    LineEditLastRead = new QLineEdit();        

    LabelProperty = new QLabel();
    LabelProperty->setText(tr("Property:"));
    
    CheckBoxIsDir = new QCheckBox();
    CheckBoxIsDir->setText(tr("Dir"));

    CheckBoxIsFile = new QCheckBox();
    CheckBoxIsFile->setText(tr("File"));

    CheckBoxIsSymLink = new QCheckBox();
    CheckBoxIsSymLink->setText(tr("SymLink"));
    
    CheckBoxIsHidden = new QCheckBox();
    CheckBoxIsHidden->setText(tr("Hidden"));
    
    CheckBoxIsReadable = new QCheckBox();
    CheckBoxIsReadable->setText(tr("Readable"));

    CheckBoxIsWritable = new QCheckBox();
    CheckBoxIsWritable->setText(tr("Writable"));

    CheckBoxIsExecutable = new QCheckBox();
    CheckBoxIsExecutable->setText(tr("Executable"));
      
    PushButtonGet = new QPushButton( this);
    PushButtonGet->setText( tr( "Get" ) );      
    
    QHBoxLayout *hbLayout1 = new QHBoxLayout ();
    hbLayout1->addWidget(LabelFileName);
    hbLayout1->addWidget(LineEditFileName);
    hbLayout1->addWidget(PushButtonFile);
    
    QHBoxLayout *hbLayout2 = new QHBoxLayout ();
    hbLayout2->addWidget(LabelSize);
    hbLayout2->addWidget(LineEditSize);

    QHBoxLayout *hbLayout3 = new QHBoxLayout ();
    hbLayout3->addWidget(LabelCreated);
    hbLayout3->addWidget(LineEditCreated);
    
    QHBoxLayout *hbLayout4 = new QHBoxLayout ();
    hbLayout4->addWidget(LabelLastModified);
    hbLayout4->addWidget(LineEditLastModified);
    
    QHBoxLayout *hbLayout5 = new QHBoxLayout ();
    hbLayout5->addWidget(LabelLastRead);
    hbLayout5->addWidget(LineEditLastRead);
    
    QHBoxLayout *hbLayout6 = new QHBoxLayout ();
    hbLayout6->addWidget(LabelProperty);
    hbLayout6->addStretch();            

    QHBoxLayout *hbLayout7 = new QHBoxLayout ();
    hbLayout7->addWidget(CheckBoxIsDir);
    hbLayout7->addWidget(CheckBoxIsFile);
    hbLayout7->addWidget(CheckBoxIsSymLink);
    hbLayout7->addWidget(CheckBoxIsHidden);
    hbLayout7->addWidget(CheckBoxIsReadable);
    hbLayout7->addWidget(CheckBoxIsWritable);
    hbLayout7->addWidget(CheckBoxIsExecutable);
      
    QVBoxLayout *vbLayout = new QVBoxLayout( this );
    vbLayout->addLayout( hbLayout1 );
    vbLayout->addLayout( hbLayout2 );
    vbLayout->addLayout( hbLayout3 );
    vbLayout->addLayout( hbLayout4 );
    vbLayout->addLayout( hbLayout5 );
    vbLayout->addLayout( hbLayout6 );
    vbLayout->addLayout( hbLayout7 );       
    vbLayout->addWidget( PushButtonGet );

	connect(PushButtonFile,SIGNAL(clicked()),this,SLOT(slotFile()));
    connect(PushButtonGet,SIGNAL(clicked()),this,SLOT(slotGet()));

}

FileInformation::~FileInformation()
{

}

void FileInformation::slotFile()
{
    QString s = QFileDialog::getOpenFileName(               
	this, "open file dialog",
        "/",
        "files (*)");            
                                    
    LineEditFileName->setText( s.toAscii() );  	
}

void FileInformation::slotGet()
{
	getFileInformation(LineEditFileName->text());
}

void FileInformation::getFileInformation(QString file)
{
	QFileInfo info(file);

	qint64 size = info.size();
	QDateTime created = info.created();
	QDateTime lastModified = info.lastModified();
	QDateTime lastRead = info.lastRead();
	bool isDir = info.isDir();
	bool isFile = info.isFile();
	bool isSymLink = info.isSymLink();
	bool isHidden = info.isHidden();
	bool isReadable = info.isReadable();
	bool isWritable = info.isWritable();
	bool isExecutable =info.isExecutable();
	
	LineEditSize->setText(QString::number(size));
	LineEditCreated->setText(created.toString());
	LineEditLastModified->setText(lastModified.toString());
	LineEditLastRead->setText(lastRead.toString());
	CheckBoxIsDir->setCheckState (isDir?Qt::Checked:Qt::Unchecked);
	CheckBoxIsFile->setCheckState (isFile?Qt::Checked:Qt::Unchecked);
	CheckBoxIsSymLink->setCheckState (isSymLink?Qt::Checked:Qt::Unchecked);
	CheckBoxIsHidden->setCheckState (isHidden?Qt::Checked:Qt::Unchecked);
	CheckBoxIsReadable->setCheckState (isReadable?Qt::Checked:Qt::Unchecked);
	CheckBoxIsWritable->setCheckState (isWritable?Qt::Checked:Qt::Unchecked);
	CheckBoxIsExecutable->setCheckState (isExecutable?Qt::Checked:Qt::Unchecked);
}

#include "fileviewer.h"

FileViewer::FileViewer( QWidget *parent, Qt::WindowFlags  f )
    : QDialog( parent, f )
{  
    setWindowTitle(tr("File Viewer"));
       
    LineEditDir = new QLineEdit();  
    LineEditDir->setText("/"); 

	ListWidgetFile = new QListWidget();
      
    QVBoxLayout *vbLayout = new QVBoxLayout( this );      
    vbLayout->addWidget( LineEditDir );
    vbLayout->addWidget( ListWidgetFile );

	connect(LineEditDir,SIGNAL(returnPressed ()),this,SLOT(slotShow()));
	connect(ListWidgetFile,SIGNAL(itemDoubleClicked(QListWidgetItem * )),this,SLOT(slotShowDir(QListWidgetItem *)));

 	QString root = "/";
 	QDir rootDir(root);	
	QStringList string;
	string << "*" ; 	
	QFileInfoList list=rootDir.entryInfoList (string);
	showFileInfoList(list);
}

FileViewer::~FileViewer()
{

}

//显示文件信息列表
void FileViewer::showFileInfoList(QFileInfoList list)
{
    ListWidgetFile->clear();
    for(unsigned int i=0;i<list.count();i++)
    {
    	QFileInfo tmpFileInfo=list.at(i);
		if((tmpFileInfo.isDir()))
		{
			QIcon icon(":/images/dir.png");

			QString fileName=tmpFileInfo.fileName();
			
			QListWidgetItem *tmp=new QListWidgetItem (icon,fileName);
			
			ListWidgetFile->addItem(tmp);

		}
		else if(tmpFileInfo.isFile())
		{
			QIcon icon(":/images/file.png");
			QString fileName=tmpFileInfo.fileName();
			QListWidgetItem *tmp=new QListWidgetItem (icon,fileName);
			ListWidgetFile->addItem(tmp);

		}
    }
}

void FileViewer::slotShowDir(QListWidgetItem * item)
{
	QString str=item->text();
	QDir dir;
	dir.setPath(LineEditDir->text());
	dir.cd(str);
	LineEditDir->setText(dir.absolutePath());
	slotShow(dir);
}

void FileViewer::slotShow(QDir dir)
{
	QStringList string;
	string << "*" ; 	
	QFileInfoList list=dir.entryInfoList (string,QDir::AllEntries,QDir::DirsFirst);
	showFileInfoList(list);	
}
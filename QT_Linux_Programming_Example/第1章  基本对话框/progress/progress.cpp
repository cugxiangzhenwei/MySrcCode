#include "progress.h"
#include <QTest>
Progress::Progress( QWidget *parent, Qt::WindowFlags  f )
    : QDialog( parent, f )
{ 
    QFont font("ZYSong18030",12);
    setFont(font);
    	 
    setWindowTitle(tr("Progress"));

    QGridLayout *layout = new QGridLayout( this );  

    numLabel = new QLabel(tr("File Num:")); 
    numLineEdit =  new QLineEdit;
    numLineEdit->setText(tr("10"));
    
    typeLabel = new QLabel(tr("Progress Type:"));
    typeComboBox = new QComboBox;
    typeComboBox->addItem(tr("ProgressBar"));
    typeComboBox->addItem(tr("ProgressDialog"));
    
    progressBar = new QProgressBar;

    startPushButton = new QPushButton(tr("Start"));
    	
    layout->addWidget( numLabel, 0, 0 );	
    layout->addWidget( numLineEdit, 0, 1 );
    layout->addWidget( typeLabel, 1, 0 );
    layout->addWidget( typeComboBox, 1, 1 );
    layout->addWidget( progressBar, 2, 0, 1, 2 );
    layout->addWidget( startPushButton, 3, 1 );
    layout->setMargin(15);
    layout->setSpacing(10);
    
    connect(startPushButton,SIGNAL(clicked()),this,SLOT(slotStart()));

}

Progress::~Progress()
{
}
                              
void Progress::slotStart()
{
	int num=numLineEdit->text().toInt();

	if(typeComboBox->currentIndex() == 0)/*ProgressBar*/
	{
		progressBar->setRange(0,num);
	    for (int i=1; i<num+1; i++)
	    {
	    	progressBar->setValue(i);
	    	/*此处模拟文件拷贝过程*/
	    	QTest::qWait(1000);
	   	}		
	}
	else if(typeComboBox->currentIndex() == 1)/*modal ProgressDialog*/
	{
		QProgressDialog *progressDialog = new QProgressDialog(this);
    	QFont font("ZYSong18030",12);
    	progressDialog->setFont(font);		
		progressDialog->setWindowModality(Qt::WindowModal);

	    progressDialog->setMinimumDuration(5);
	    progressDialog->setWindowTitle(tr("Please Wait"));
	    progressDialog->setLabelText(tr("Copying..."));
	    progressDialog->setCancelButtonText(tr("Cancel"));		
	    progressDialog->setRange(0,num);
		
	    for (int i=1; i<num+1; i++)
	    {
	    	progressDialog->setValue(i);
	    	qApp->processEvents();
	    	/*此处模拟文件拷贝过程*/
	    	QTest::qWait(1000);    	    
	    	if (progressDialog->wasCanceled())
	    	    return;
	   	}			
	}

}


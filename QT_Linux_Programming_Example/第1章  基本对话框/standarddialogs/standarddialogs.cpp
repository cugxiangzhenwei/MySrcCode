#include "standarddialogs.h"

StandardDialogs::StandardDialogs( QWidget *parent, Qt::WindowFlags  f )
    : QDialog( parent, f )
{  
    setWindowTitle(tr("Standard Dialogs"));

    layout = new QGridLayout( this );  
    
    filePushButton = new QPushButton();
    filePushButton->setText(tr("File Dialog"));
    
    colorPushButton = new QPushButton();
    colorPushButton->setText(tr("Color Dialog"));
    
    fontPushButton = new QPushButton( );
    fontPushButton->setText(tr("Font Dialog"));

    fileLineEdit = new QLineEdit();
    
    colorFrame = new QFrame();
    colorFrame->setFrameShape( QFrame::Box );
    colorFrame->setAutoFillBackground(true);
    
    fontLineEdit = new QLineEdit( );
    fontLineEdit->setText(tr("Hello World"));
    	
    layout->addWidget( filePushButton, 0, 0 );	
    layout->addWidget( fileLineEdit, 0, 1 );
    layout->addWidget( colorPushButton, 1, 0 );
    layout->addWidget( colorFrame, 1, 1 );
    layout->addWidget( fontPushButton, 2, 0 );
    layout->addWidget( fontLineEdit, 2, 1 );
    layout->setMargin(15);
    layout->setSpacing(10);
    
    connect(filePushButton,SIGNAL(clicked()),this,SLOT(slotOpenFileDlg()));
    connect(colorPushButton,SIGNAL(clicked()),this,SLOT(slotOpenColorDlg()));
    connect(fontPushButton,SIGNAL(clicked()),this,SLOT(slotOpenFontDlg()));
}

StandardDialogs::~StandardDialogs()
{
}
                              
void StandardDialogs::slotOpenFileDlg()
{                              
    QString s = QFileDialog::getOpenFileName(               
	this, "open file dialog",
        "/",
        "C++ files (*.cpp);;C files (*.c);;Head files (*.h)");            
                                    
    fileLineEdit->setText( s.toAscii() );         
}                                                           
                                                            
void StandardDialogs::slotOpenColorDlg()                    
{                                                           
    QColor color = QColorDialog::getColor (Qt::blue);    
    
    if(color.isValid())
    {
		colorFrame->setPalette(QPalette(color));
    }
}                                                           
                                                            
void StandardDialogs::slotOpenFontDlg()                     
{                             
    bool ok;                  
                              
    QFont font = QFontDialog::getFont( &ok );
    
    if( ok ) 
    {
    	fontLineEdit->setFont( font );
    }

}
#include "eventfilter.h"

EventFilter::EventFilter( QWidget *parent, Qt::WindowFlags  f )
    : QDialog( parent, f )
{  
    QFont font("ZYSong18030",12);
    setFont(font);	
    
    setWindowTitle(tr("Event Filter"));
    
    Image1.load(":/images/1.png");
    Image2.load(":/images/2.png");
    Image3.load(":/images/3.png");
    
    Label1 = new QLabel( this );
    Label1->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
	Label1 ->setPixmap(QPixmap::fromImage(Image1));
    Label2 = new QLabel( this );
    Label2->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
	Label2 ->setPixmap(QPixmap::fromImage(Image2));
    Label3 = new QLabel( this );
    Label3->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
	Label3 ->setPixmap(QPixmap::fromImage(Image3));
   
   	LabelState = new QLabel( this );
   	LabelState->setAlignment(Qt::AlignHCenter);
   
    QHBoxLayout *hbLayout = new QHBoxLayout();
    hbLayout->addWidget( Label1 );
    hbLayout->addWidget( Label2 );
    hbLayout->addWidget( Label3 );

    QVBoxLayout *vbLayout = new QVBoxLayout(this);
    vbLayout->addLayout( hbLayout );
    vbLayout->addWidget( LabelState );

	Label1->installEventFilter(this);
	Label2->installEventFilter(this);
	Label3->installEventFilter(this);

}

EventFilter::~EventFilter()
{
}

bool EventFilter::eventFilter(QObject* watched,QEvent* event)
{
	if(watched == Label1)
	{
		if(event->type() == QEvent::MouseButtonPress)
		{
			QMouseEvent *mouseEvent = (QMouseEvent *)event;	
			if(mouseEvent->buttons() &Qt::LeftButton)
			{
				LabelState->setText(tr("Left mouse button pressed on left image"));
			}
			else if(mouseEvent->buttons() &Qt::MidButton)
			{
				LabelState->setText(tr("Middle mouse button pressed on left image"));
			}	
			else if(mouseEvent->buttons() &Qt::RightButton)
			{
				LabelState->setText(tr("Right mouse button pressed on left image"));
			}					
			QMatrix martix;
			martix.scale(0.8,0.8);  
			QImage tmp=Image1.transformed(martix);
			Label1->setPixmap (QPixmap::fromImage(tmp));

		}		
		if(event->type() == QEvent::MouseButtonRelease)
		{
			LabelState->setText(tr("Mouse button released from left image"));
			Label1->setPixmap (QPixmap::fromImage(Image1));
		}	

	}
	if(watched == Label2)
	{
		if(event->type() == QEvent::MouseButtonPress)
		{
			QMouseEvent *mouseEvent = (QMouseEvent *)event;
			if(mouseEvent->buttons() &Qt::LeftButton)
			{
				LabelState->setText(tr("Left mouse button pressed on middle image"));
			}
			else if(mouseEvent->buttons() &Qt::MidButton)
			{
				LabelState->setText(tr("Middle mouse button pressed on middle image"));
			}	
			else if(mouseEvent->buttons() &Qt::RightButton)
			{
				LabelState->setText(tr("Right mouse button pressed on middle image"));
			}								
			QMatrix martix;
			martix.scale(0.8,0.8);  
			QImage tmp=Image2.transformed(martix);
			Label2->setPixmap (QPixmap::fromImage(tmp));
		}		
		if(event->type() == QEvent::MouseButtonRelease)
		{
			LabelState->setText(tr("Mouse button released from middle image"));
			Label2->setPixmap (QPixmap::fromImage(Image2));
		}
	}
	if(watched == Label3)
	{
		if(event->type() == QEvent::MouseButtonPress)
		{
			QMouseEvent *mouseEvent = (QMouseEvent *)event;
			if(mouseEvent->buttons() &Qt::LeftButton)
			{
				LabelState->setText(tr("Left mouse button pressed on right image"));
			}
			else if(mouseEvent->buttons() &Qt::MidButton)
			{
				LabelState->setText(tr("Middle mouse button pressed on right image"));
			}	
			else if(mouseEvent->buttons() &Qt::RightButton)
			{
				LabelState->setText(tr("Right mouse button pressed on right image"));
			}			
			QMatrix martix;
			martix.scale(0.8,0.8);  
			QImage tmp=Image3.transformed(martix);
			Label3->setPixmap (QPixmap::fromImage(tmp));
		}		
		if(event->type() == QEvent::MouseButtonRelease)
		{
			LabelState->setText(tr("Mouse button released from right image"));
			Label3->setPixmap (QPixmap::fromImage(Image3));
		}
	}
	return QDialog::eventFilter(watched,event);	
}
                                                                             

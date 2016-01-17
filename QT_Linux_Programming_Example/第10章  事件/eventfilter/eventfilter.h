#ifndef EVENTFILTER_H
#define EVENTFILTER_H

#include <QtGui>

class EventFilter : public QDialog
{
    Q_OBJECT
public:
    EventFilter( QWidget *parent=0, Qt::WindowFlags  f=0 );
    ~EventFilter();
public:
	QLabel* Label1;
	QLabel* Label2;
	QLabel* Label3;
	QLabel* LabelState;
	
	QImage Image1;
	QImage Image2;
	QImage Image3;

public slots:
	bool eventFilter(QObject*,QEvent*);
};


#endif

#ifndef NETWORKINFORMATION_H
#define NETWORKINFORMATION_H

#include <QtGui>
#include <QtNetwork>

class NetworkInformation : public QDialog		// network information dialogs widget
{
    Q_OBJECT
public:
    NetworkInformation( QWidget *parent=0, Qt::WindowFlags  f=0 );
    ~NetworkInformation();
public:
	QLabel* LabelLocalHostName;
	QLineEdit* LineEditLocalHostName;
	QLabel* LabelAddress;
	QLineEdit* LineEditAddress;
    QPushButton* PushButtonDetail;
public:
	void getHostInformation();
public slots:
    void slotDetail();
};


#endif // NETWORKINFORMATION_H

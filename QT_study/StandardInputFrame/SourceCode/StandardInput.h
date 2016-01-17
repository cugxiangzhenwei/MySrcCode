#ifndef STANDARDINPUT_H
#define STANDARDINPUT_H
#include <QtGui>

class CStandInput:public QDialog
{
	Q_OBJECT
public :
	CStandInput();
	~CStandInput();
public:
	QPushButton *nameButton;
	QPushButton *sexButton;
	QPushButton *ageButton;
	QPushButton *statureButton;

	QLabel *labelName;
	QLabel *labelSex;
	QLabel *labelAge;
	QLabel *labelStature;

	QLabel *label1;
	QLabel *label2;
	QLabel *label3;
	QLabel *label4;
private slots:
		void SetName();
		void SetSex();
		void SetAge();
		void SetStature();
};
#endif

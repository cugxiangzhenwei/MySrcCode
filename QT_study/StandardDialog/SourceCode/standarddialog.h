#ifndef STANDARDDIALOG_H
#define STANDARDDIALOG_H

#include <QDialog>
#include <QGridLayout>
#include <QPushButton>
#include <QLineEdit>

#include <string>
using namespace std;

class CStandardDialog: public QDialog
{
	Q_OBJECT
public:
	CStandardDialog(QWidget *parent=0,Qt::WindowFlags f=0);
	~CStandardDialog(){}
protected:
	QGridLayout *layout;
	QPushButton *filePushButton;
	QPushButton *colorPushButton;
	QPushButton *fontPushButton;
	QLineEdit  * fileLineEdit;
	QLineEdit  * fontLineEdit;
	QFrame     * colorFrame;
private slots:
	void OpenFileDlg();
	void OpenColorDlg();
	void OpenFontDlg();
};
#endif
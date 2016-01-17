#ifndef TESTDLG_H
#define TESTDLG_H
#include <QDialog>
#include <QtGui>
#include "ui_TestDlg.h"

class CTestDlg :public QDialog,private Ui::TestDlg
{
	Q_OBJECT
public:
	CTestDlg (QWidget *parent=0);
	private slots:
		void OpenFile();
		void OutFile();
};
#endif
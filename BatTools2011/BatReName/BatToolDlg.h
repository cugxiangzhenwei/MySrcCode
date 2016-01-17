#ifndef BATTOOLDLG_H
#define BATTOOLDLG_H
#include "ui_BatToolDlg.h"
#include <QDialog>
#include "BatFileSystem.h"
#include <Windows.h>
//#pragma comment(lib,"BatLib.lib")

class CBatReNameDlg:public QDialog,private Ui::BatReNameDlg
{
	Q_OBJECT
public:
		CBatReNameDlg(QWidget *parent=0);
		~CBatReNameDlg();
public slots:
	void OpenDir();
	void ReName();
private:
	string m_strDir;
	string m_strExt;
	string m_strPre;
};
#endif
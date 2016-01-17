#include <QApplication>
#include "standarddialog.h"
#include <QTextCodec>
int main(int argc,char *argv[])
{
	QApplication app(argc,argv);

	//中文支持
	QTextCodec *codec=QTextCodec::codecForName("GBK");
	if (codec==NULL)
	{
		codec=QTextCodec::codecForLocale();
	}
	QTextCodec::setCodecForLocale(codec);
	QTextCodec::setCodecForTr(codec);
	QTextCodec::setCodecForCStrings(codec);

	CStandardDialog *pDlg=new CStandardDialog;
	pDlg->show();

	return app.exec();
}
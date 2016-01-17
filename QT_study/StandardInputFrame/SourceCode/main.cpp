#include "StandardInput.h"
#include <QApplication>
#include <QTextCodec>
int main(int argc, char *argv[])
{
	QApplication myapp(argc,argv);

	//中文支持
	QTextCodec *codec=QTextCodec::codecForName("GBK");
	if (codec==NULL)
	{
		codec=QTextCodec::codecForLocale();
	}
	QTextCodec::setCodecForLocale(codec);
	QTextCodec::setCodecForTr(codec);
	QTextCodec::setCodecForCStrings(codec);

	CStandInput *pInputDlg=new CStandInput;
	pInputDlg->show();

	myapp.exec();
}
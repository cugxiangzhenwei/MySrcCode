#include <QApplication>
#include <QTextCodec>
#include "TestDlg.h"
int main(int argc ,char *argv[])
{
	QApplication app(argc, argv);

	QTextCodec *codec=QTextCodec::codecForName("GBK");
	if (codec==NULL)
	{
		codec=QTextCodec::codecForLocale();
	}
	QTextCodec::setCodecForLocale(codec);
	QTextCodec::setCodecForTr(codec);
	QTextCodec::setCodecForCStrings(codec);

	CTestDlg *pdlg=new CTestDlg;
	pdlg->show();
	return app.exec();
}
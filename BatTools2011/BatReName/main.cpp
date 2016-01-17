#include "BatToolDlg.h"
#include <QTextCodec>

int main(int argc, char *argv[])
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

	CBatReNameDlg *dlg=new CBatReNameDlg;
	dlg->show();
	return app.exec();
}
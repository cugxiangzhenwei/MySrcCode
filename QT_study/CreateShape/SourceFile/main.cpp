#include <QApplication>
#include <QDialog>
#include <QTextCodec>
#include "CreatShip.h"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
	//QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GBK"));
	QTextCodec *codec=QTextCodec::codecForName("GBK");
	if (codec==NULL)
	{
		codec=QTextCodec::codecForLocale();
	}
	QTextCodec::setCodecForLocale(codec);
	QTextCodec::setCodecForTr(codec);
	QTextCodec::setCodecForCStrings(codec);

    CreatShip *dlg=new CreatShip(0);
	dlg->show();
    return app.exec();
}

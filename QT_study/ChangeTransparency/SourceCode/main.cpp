#include "changetransparency.h"

int main(int argc,char *argv[])
{
	QApplication app(argc,argv);

	CTransParency *pDlg=new CTransParency;
	pDlg->show();

	app.exec();
}
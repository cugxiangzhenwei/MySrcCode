#include <QApplication>
#include <Qstring>
#include <QRegExp>
#include <QMessageBox>
#include <QStringList>
#include <iostream>
using namespace std;

int main(int argc,char *argv[])
{
	QApplication app(argc,argv);

	cout<<"Hello World!"<<endl;

	QString str = " 0 ,0.1 ,3,2.3,5.5,6,9 ";

	QString strRegExp = "\\s*[0-9]+\\.?[0-9]*\\s*,\\s*[0-9]+\\.?[0-9]*\\s*,\\s*[0-9]+\\.?[0-9]*\\s*,\\s*[0-9]+\\.?[0-9]*\\s*,\\s*[0-9]+\\.?[0-9]*\\s*,\\s*[0-9]+\\.?[0-9]*\\s*,\\s*[0-9]+\\.?[0-9]*\\s*";

	QRegExp rx(strRegExp, Qt::CaseSensitive);    // primitive floating point matching

	bool bOK = rx.exactMatch(str);
	if (!bOK)
	{
		QMessageBox::information(NULL,"Error","Err Input!");
		return app.exec();
	}
	else
	{
		QStringList strList = str.split(",");
	}

	return app.exec();
}
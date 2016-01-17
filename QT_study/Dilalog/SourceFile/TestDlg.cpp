#include "TestDlg.h"

CTestDlg::CTestDlg(QWidget *parent/* =0 */):QDialog(parent)
{
	setupUi(this);
	connect(pushButtonInFile,SIGNAL(clicked()),this,SLOT(OpenFile()));
	connect(pushButtonOutFile,SIGNAL(clicked()),this,SLOT(OutFile()));
}
void CTestDlg::OpenFile()
{
	static QString selectedfilter;
	QString InFileName = QFileDialog::getOpenFileName(0, tr("打开影像文件"),lineEditInFile->text(),
		tr("BMP File(*.bmp);;JPG File(*.jpg);;GeoTif File (*.tif);;IMG File (*.img)"),&selectedfilter);
	if (InFileName!=NULL)
	{
		lineEditInFile->setText(InFileName);
	}
}
void CTestDlg::OutFile()
{
	static QString selectedfilter;
	QString OutFileName = QFileDialog::getSaveFileName(0, tr("保存影像"), lineEditOutFile->text(),
		tr("BMP File(*.bmp);;JPG File(*.jpg);;GeoTif File (*.tif);;IMG File (*.img)"),&selectedfilter);
	lineEditOutFile->setText(OutFileName);
}
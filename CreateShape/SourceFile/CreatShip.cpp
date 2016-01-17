#include <QtGui>
#include "CreatShip.h"
#include "BuildShape.h"
#include <string>
using namespace std;
CreatShip::CreatShip(QWidget *parent):QDialog(parent)
{
	setupUi(this);
	CreateBtn->setEnabled(false);
	connect(OpenRasterBtn,SIGNAL(clicked()),SLOT(OpenRaster()));
	connect(SaveShapeBtn,SIGNAL(clicked()),SLOT(SaveShape()));
	connect(CreateBtn,SIGNAL(clicked()),SLOT(accept()));
	connect(CancelBtn,SIGNAL(clicked()),SLOT(reject()));
	connect(ShapName,SIGNAL(textChanged(const QString &)),this,SLOT(EnableOkBtn(const QString &)));
	connect(CreateBtn,SIGNAL(clicked()),SLOT(CreateBtnclicked()));
}
void CreatShip::OpenRaster()
{
	QString strRasterName = QFileDialog::getOpenFileName(0, tr("打开栅格数据"), QString(),
		tr("IMG File(*.img);;Tiff File(*.tif)"));
	RasterName->setText(strRasterName);
}
void CreatShip::SaveShape()
{
	QString strShapeName = QFileDialog::getSaveFileName(0, tr("保存矢量数据"), QString(),
		tr("矢量层文件(*.shp )"));
	ShapName->setText(strShapeName);
}
void CreatShip::EnableOkBtn(const QString &text)
{
	CreateBtn->setEnabled(!text.isEmpty());
}
void CreatShip::CreateBtnclicked()
{
	QString QstrRasterName=RasterName->text();
	char *pszRasterName;
	string StingName=QstrRasterName.toStdString();
	pszRasterName=new char[StingName.size()+1];
	strcpy(pszRasterName,StingName.c_str());
	int rows=SpinBoxRows->text().toUInt();
	int clos=SpinBoxCols->text().toUInt();

	QString QstrShipName=ShapName->text();
	char *pszShipName;
	string StingName2=QstrShipName.toStdString();
	pszShipName=new char[StingName2.size()+1];
	strcpy(pszShipName,StingName2.c_str());
	if (build(pszRasterName,pszShipName,rows,clos))
	{
		QMessageBox::information(this,"提示","创建矢量成功!",QMessageBox::Ok);
	}
}
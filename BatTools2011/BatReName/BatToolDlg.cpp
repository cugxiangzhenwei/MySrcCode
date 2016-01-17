#include "BatToolDlg.h"
#include <QString>
#include <QFileDialog>
#include <QMessageBox>
#include <string>
using namespace std;

CBatReNameDlg::CBatReNameDlg(QWidget *parent):QDialog(parent)
{
	setupUi(this);
	progressBar->setValue(0);
	QObject::connect(pushButtonOpen,SIGNAL(clicked()),this,SLOT(OpenDir()));
	QObject::connect(pushButtonReName,SIGNAL(clicked()),this,SLOT(ReName()));
	QObject::connect(pushButtonQuit,SIGNAL(clicked()),this,SLOT(reject()));
	QPixmap pixmap("backimg.png");
	QPalette palette;
	palette.setBrush(this->backgroundRole(),QBrush(pixmap));
	this->setPalette(palette);
	//this->setMask(pixmap.mask());//将图片中的透明部分显示出来
	this->setAutoFillBackground(true);
}
CBatReNameDlg::~CBatReNameDlg()
{

}
void CBatReNameDlg::OpenDir()
{
	QFileDialog::Options options = QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly;

	QString directory = QFileDialog::getExistingDirectory(this,
		tr("批量命名文件路径"),lineEditDir->text(),options);

	if (!directory.isEmpty())
	{
		lineEditDir->setText(directory);
		m_strDir=directory.toStdString();
	}
}
void CBatReNameDlg::ReName()
{
	m_strDir=lineEditDir->text().toStdString();
	path dir(m_strDir);
	if (dir.empty())
	{
		QMessageBox::information(this,"提示","输入路径不存在",QMessageBox::Ok);
		return;
	}
	m_strExt=lineEdit_Ext->text().toStdString();
	m_strPre=lineEdit_Pre->text().toStdString();
	if (m_strExt=="")
	{

		QMessageBox::information(this,"提示","请输入要重命名的文件类型",QMessageBox::Ok);
		return;
	}
	vector<path>v;
	string strEx="."+m_strExt;
	label_pro->setText("正在查找文件");
	FindExtenFile(dir,v,strEx,true);
	int isize=v.size();
	if (isize==0)
	{
		label_pro->setText("进度");
		QMessageBox::information(this,"提示","未找到指定格式的文件",QMessageBox::Ok);
		return;
	}
	progressBar->setRange(0,isize);
	int num=0;
	label_pro->setText("正在重命名");
	pushButtonOpen->setEnabled(false);
	pushButtonReName->setEnabled(false);
	pushButtonQuit->setEnabled(false);
	for (int i=0;i<isize;i++)
	{
		progressBar->setValue(i+1);
		QString QstrName=QString("%1").arg(i);
		string strName=m_strPre+QstrName.toStdString()+"."+ m_strExt;
		path FileName(strName);
		path des=v[i].parent_path()/FileName;
		QString strInfoName=QString(des.string().c_str());
		QFileInfo qInfo(strInfoName);
		if (qInfo.exists()&&qInfo.isFile())
			continue;
		rename(v[i],des);
		num=num+1;
	}
	pushButtonOpen->setEnabled(true);
	pushButtonReName->setEnabled(true);
	pushButtonQuit->setEnabled(true);
	label_pro->setText("重命名完成");
	QString str=QString("共找到文件%1个，共处理文件%2个").arg(isize).arg(num);
	QMessageBox::information(this,"提示",str,QMessageBox::Ok);
	//BatReName(v,m_strPre); //不好实现进度条
}
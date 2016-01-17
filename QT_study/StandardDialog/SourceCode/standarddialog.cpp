#include"standarddialog.h"
#include <QtGui>

CStandardDialog::CStandardDialog(QWidget *parent,Qt::WindowFlags f):QDialog(parent,f)
{
	setWindowTitle("标准对话框");

	layout=new QGridLayout(this);

	filePushButton=new QPushButton;
	filePushButton->setText("文件对话框");

	colorPushButton=new QPushButton;
	colorPushButton->setText("颜色对话框");

	fontPushButton=new QPushButton;
	fontPushButton->setText("字体对话框");

	fileLineEdit =new QLineEdit;

	colorFrame=new QFrame;
	colorFrame->setFrameShape(QFrame::Box);
	colorFrame->setAutoFillBackground(true);

	fontLineEdit=new QLineEdit;
	fontLineEdit->setText("欢迎来访二十一世纪公司!");

	
	layout->addWidget(fileLineEdit,0,0);
	layout->addWidget(filePushButton,0,1);
	layout->addWidget(colorFrame,1,0);
	layout->addWidget(colorPushButton,1,1);
	layout->addWidget(fontLineEdit,2,0);
	layout->addWidget(fontPushButton,2,1);
	
	layout->setMargin(15);
	layout->setSpacing(10);
	resize(300,150);
	connect(filePushButton,SIGNAL(clicked()),this,SLOT(OpenFileDlg()));
	connect(colorPushButton,SIGNAL(clicked()),this,SLOT(OpenColorDlg()));
	connect(fontPushButton,SIGNAL(clicked()),this,SLOT(OpenFontDlg()));
}

void CStandardDialog::OpenFileDlg()
{
	QString strFilter=tr("C++ Files(*.cpp);;C Files(*.c);;Head Files(*.h)");
	QString str=QFileDialog::getOpenFileName(this,"Open File Dialog","/",strFilter,0,0);
	fileLineEdit->setText(str.toAscii());
}
void CStandardDialog::OpenColorDlg()
{
	QColor color=QColorDialog::getColor(Qt::blue);
	if (color.isValid())
	{
		colorFrame->setPalette(QPalette(color));
	}
}
void CStandardDialog::OpenFontDlg()
{

	bool ok;
	QFont font=QFontDialog::getFont(&ok);
	if (ok)
	{
		fontLineEdit->setFont(font);
	}
}
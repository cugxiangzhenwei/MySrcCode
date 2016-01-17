#include "BuildShape.h"
#include <QMessageBox>
bool build(const char* pszFileName ,const char *outdataName,int linenum,int rows)
{

	//获取影像信息
	GDALDataset *poDataSet;
	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8","NO");
	poDataSet=(GDALDataset*)GDALOpen(pszFileName,GA_ReadOnly); //打开数据集
	if (poDataSet==NULL)
	{
		QMessageBox::critical(NULL,"错误提示","打开栅格数据集失败!",QMessageBox::Ok);
		return false;
	}
	double Trans[6];//坐标转换参数数组,作为GetGeoTransform()函数的参数
	int width=poDataSet->GetRasterXSize(); //获取影像列数，为后续划分网格做准备
	int height=poDataSet->GetRasterYSize();//获取影像行数，为后续划分网格做准备
	poDataSet->GetGeoTransform(Trans);
	//Xp = padfTransform[0] + P*padfTransform[1] + L*padfTransform[2];
	//Yp = padfTransform[3] + P*padfTransform[4] + L*padfTransform[5];
	//In a north up image, padfTransform[1] is the pixel width, and padfTransform[5] is the pixel height.
	//The upper left corner of the upper left pixel is at position (padfTransform[0],padfTransform[3]).
	//注册shape文件驱动
	const char* pszDriverName="ESRI Shapefile";
	OGRSFDriver *poDriver;
	OGRRegisterAll();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8","NO");
	poDriver=OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName(pszDriverName);
	if (poDriver==NULL)
	{
		QMessageBox::critical(NULL,"错误提示","注册文件驱动失败!",QMessageBox::Ok);
		return false;

	}
	//创建shape文件
	OGRDataSource *poDS;
	//创建一个叫myshapefile的目录，存放生成的文件
	//如果名字有.shp后缀，则直接在当前目录下生成文件
	poDS=poDriver->CreateDataSource(outdataName,NULL); 
	if (poDS==NULL)
	{
		QMessageBox::critical(NULL,"错误提示","创建矢量数据文件失败!",QMessageBox::Ok);
		return false;
	}
	//创建输出图层
	OGRLayer *poLayer;
	const char *prj=poDataSet->GetProjectionRef(); //获取栅格影像的空间参考信息
	//cout<<"栅格数据空间参考信息为:\n"<<prj<<endl<<endl;
	OGRSpatialReference oSRS;  
	oSRS.SetFromUserInput(prj);  //将获得的空间参考信息字符串做为文本一次性赋给矢量数据的OGRSpatialReference对象
	poLayer=poDS->CreateLayer(outdataName,&oSRS);
	if (poLayer==NULL)
	{
		QMessageBox::critical(NULL,"错误提示","创建矢量层失败!",QMessageBox::Ok);
		return false;
	}
	//创建层数据的属性fields
	OGRFieldDefn oField("Point",OFTString);
	oField.SetWidth(10);
	if (poLayer->CreateField(&oField)!=OGRERR_NONE)
	{
		QMessageBox::critical(NULL,"错误提示","创建层数据域field失败!",QMessageBox::Ok);
		return false;
	}
	//创建features，写入feature到磁盘
	OGRFeature *poFeature;
	poFeature=OGRFeature::CreateFeature(poLayer->GetLayerDefn());
	//绘制外边框
	OGRLineString Line;
	OGRPoint Point1(Trans[0],Trans[3]); 
	OGRPoint Point2(Trans[0]+width*Trans[1],Trans[3]);
	OGRPoint Point3(Trans[0]+width*Trans[1],Trans[3]+width*Trans[4]+height*Trans[5]);
	OGRPoint Point4(Trans[0],Trans[3]+width*Trans[4]+height*Trans[5]);
	Line.addPoint(&Point1);
	Line.addPoint(&Point2);
	Line.addPoint(&Point3);
	Line.addPoint(&Point4);
	Line.addPoint(&Point1);
	//水平方向加线
	OGRLineString SubHline[50];
	OGRPoint PointLeft[50],PointRight[50];
	for (int i=1;i<linenum;i++)
	{
		PointLeft[i].setX(Trans[0]); //设置左边框上要加线的起点X坐标
		PointLeft[i].setY((Point4.getY()-Trans[3])/linenum*i+Trans[3]);//设置左边框上要加线的起点Y坐标
		PointRight[i].setX(Point2.getX());//设置右边框上要加线的起点X坐标
		PointRight[i].setY((Point4.getY()-Trans[3])/linenum*i+Trans[3]);//设置右边框上要加线的起点Y坐标
	}
	for (int i=1;i<linenum;i++)
	{  
		SubHline[i].addPoint(&PointLeft[i]); //左边框上加点
		SubHline[i].addPoint(&PointRight[i]);//右边框上加点
		if (i<linenum-1)
		{
			SubHline[i].addPoint(&PointRight[i+1]); //从右边框的上一点转到下一点，避免交叉斜线的出现
		}
		Line.addSubLineString(&SubHline[i]);//将SubHline数组中的每一个线做为子线段添加到Line对象中
	}
	Line.addPoint(&Point2);
	//垂直方向加线
	OGRLineString SubVline[50];
	OGRPoint PointUp[50],PointDown[50];
	for (int j=1;j<rows;j++)
	{
		//添加上边框上的点
		PointUp[j].setX((Point2.getX()-Trans[0])/rows*j+Trans[0]);
		PointUp[j].setY(Trans[3]);
		//添加下边框上的点
		PointDown[j].setX((Point2.getX()-Trans[0])/rows*j+Trans[0]);
		PointDown[j].setY(Point4.getY());
	}
	for (int j=1;j<rows;j++)
	{
		SubVline[j].addPoint(&PointUp[j]);
		SubVline[j].addPoint(&PointDown[j]);
		if (j<rows-1)
		{
			SubVline[j].addPoint(&PointDown[j+1]); //从下边框的前一点转到后一点，避免交叉斜线的出现
		}
		Line.addSubLineString(&SubVline[j]);
	}
	poFeature->SetGeometryDirectly(&Line);
	if (poLayer->CreateFeature(poFeature)!=OGRERR_NONE)
	{
		QMessageBox::critical(NULL,"错误提示","将feature写入矢量文件失败!",QMessageBox::Ok);
		return false;
	}
	OGRFeature::DestroyFeature(poFeature);
	OGRDataSource::DestroyDataSource(poDS);
	//printf("创建矢量数据成功！\n");
	return true;
}
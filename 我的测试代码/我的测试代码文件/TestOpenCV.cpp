#include "..\..\3rdPart\OpenCV2.1\include\opencv\cxcore.h"
#include "..\..\3rdPart\OpenCV2.1\include\opencv\cxcore.hpp"
#include "..\..\3rdPart\OpenCV2.1\include\opencv\highgui.h"
#include "..\..\3rdPart\OpenCV2.1\include\opencv\cv.h"
#include <gdal_priv.h>
#include <vld.h>

#pragma comment(lib,"D:\\RsSrcDir\\3rdPart\\OpenCV2.1\\lib\\highgui210.lib")
#pragma comment(lib,"D:\\RsSrcDir\\3rdPart\\OpenCV2.1\\lib\\cxcore210.lib")

#define  STAT_ROW  5000
#define  START_COL 5000
#define  ROW       8000
#define  COL       8000

IplImage  * GetDataFromImg(const char *pszImg,bool bIsColor)
{
	GDALAllRegister();
	GDALDatasetH hDs = GDALOpen(pszImg,GA_ReadOnly);
	if (hDs == NULL)
		return NULL;

	int nXSize = GDALGetRasterXSize(hDs);
	int nYSize = GDALGetRasterYSize(hDs);

	GDALRasterBandH hBand = GDALGetRasterBand(hDs,1);
	unsigned char *pData = NULL;
	if (!bIsColor)
	{
		pData = new unsigned char [ROW*COL];
		GDALRasterIO(hBand, GF_Read, STAT_ROW, START_COL, ROW, COL, pData, ROW, COL, GDT_Byte, NULL, NULL);
	}
	else
	{
		pData = new unsigned char [ROW*COL*3];
		int pBandMap[3]={1,2,3};
		GDALDatasetRasterIO(hDs,GF_Read, STAT_ROW, START_COL, ROW, COL, pData, ROW, COL, GDT_Byte,3,pBandMap,3,3*COL,1);
	}
	
	GDALClose(hDs);

	cv::Mat mat(ROW,COL,CV_8UC3,pData);
	IplImage  limg =(IplImage)mat;
	IplImage * img = (IplImage *)calloc(1,sizeof(IplImage) ) ;
	memcpy(img,&limg,sizeof(IplImage));
	return img;
}
void TestCV()
{
	const char *pszImg = "D:\\data717\\原始影像\\ZY02C_HRC_E116.7_N40.0_20120201_L1C0000161390.tif";
	const char *pszImg2 = "C:\\Documents and Settings\\Administrator\\My Documents\\My Pictures\\桌面背景\\PrimulaRed.jpg";
	const char *pszImg3 = "D:\\data717\\原始影像\\ZY02C_HRC_E116.7_N40.0_20120201_L1C0000161390.tiff";
	const char * pszImg4 = "E:\\02C-2C\\64965输出\\匀色输出.img";

	/*void * pData = GetDataFromImg(pszImg4,true);
	cv::Mat img(ROW,COL,CV_8UC3,pData);
	IplImage  limg =(IplImage)img;*/
	IplImage *limg = GetDataFromImg(pszImg4,true);
	cvNamedWindow("TestOpenCVandGdalWnd",CV_WINDOW_AUTOSIZE);
	cvShowImage("TestOpenCVandGdalWnd",limg);
	//delete[] limg->imageData;
	free(limg);
	cvWaitKey();
}
void TestCV2()
{
		const char *pszImg = "E:\\02C-2C\\64965输出\\64965_2.tif";
		const char *pszImg2 = "C:\\Documents and Settings\\Administrator\\My Documents\\My Pictures\\桌面背景\\PrimulaRed.jpg";
		const char *pszImg3 = "D:\\data717\\原始影像\\ZY02C_HRC_E116.7_N40.0_20120201_L1C0000161390.tiff";
		CvvImage cvimg;
		CvRect rect = cvRect(100,100,500,500);
		cvimg.LoadRect(pszImg2,1,rect);
		//cvNamedWindow("TestOpenCVandGdalWnd",CV_WINDOW_AUTOSIZE);
		//cvimg.Show("TestOpenCVandGdalWnd");
		
		IplImage * img =cvimg.GetImage();
		if (img ==NULL)
		{
			printf("构建图像失败!");
			return;
		}
	
		cvNamedWindow("TestOpenCVandGdalWnd",CV_WINDOW_AUTOSIZE);
		cvShowImage("TestOpenCVandGdalWnd",img);
		cvWaitKey();
}
void TestRGB()
{
	const char *pszImg1 = "E:\\对比照片\\图片分组\\图片分组\\不同日期同一时间拍摄\\1天\\20120814-20120815-0939\\01\\1.jpg";

	CvMat * Mat = cvLoadImageM(pszImg1,1);

	/*double dValue = cvmGet(Mat,100,100);
	printf("像素值为%f\n",dValue);*/
	cvNamedWindow("1",CV_WINDOW_AUTOSIZE);
	cvShowImage("1",Mat);

	cvWaitKey(0);
}
void main2232()
{
	TestRGB();
}
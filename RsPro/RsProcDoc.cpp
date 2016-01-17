// RsProcDoc.cpp : implementation of the CRsProcDoc class
//

#include "stdafx.h"
#include "ChildFrm.h"
#include "RsProc.h"
#include "RsProcView.h"
#include "HistDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRsProcDoc

IMPLEMENT_DYNCREATE(CRsProcDoc, CDocument)

BEGIN_MESSAGE_MAP(CRsProcDoc, CDocument)
	//{{AFX_MSG_MAP(CRsProcDoc)
	ON_COMMAND(ID_HIST_SHOW, OnHistShow)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_SAVE_AS, &CRsProcDoc::OnFileSaveAs)
	ON_COMMAND(ID_FILE_SAVE, &CRsProcDoc::OnFileSave)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRsProcDoc construction/destruction

CRsProcDoc::CRsProcDoc()
{
	// TODO: add one-time construction code here
	m_nbands=0;
	m_nHeigth=0;
	m_nWidth=0;
	for (int i=0;i<3;i++)
	{
		max[i]=0;
		max_grey[i]=0;
		min_grey[i]=0;
		for (int j=0;j<256;j++)
		{
			count[i][j]=0;
		}
	}
}

CRsProcDoc::~CRsProcDoc()
{
}

BOOL CRsProcDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CRsProcDoc serialization

void CRsProcDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CRsProcDoc diagnostics

#ifdef _DEBUG
void CRsProcDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CRsProcDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG
/**
*	@brief	 函数OnOpenDocument打开文件
*	@param	 lpszPathName  打开的文件路径名
*	@return	 返回打开文档是否成功,成功则为TRUE,否则为FALSE
*/
BOOL CRsProcDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO: Add your specialized creation code here
	CFile file;
	CFileException fe;
	if (!file.Open(lpszPathName,CFile::modeRead|CFile::shareDenyWrite,&fe))
	{
		ReportSaveLoadException(lpszPathName,&fe,FALSE,AFX_IDP_FAILED_TO_OPEN_DOC);
		return FALSE;
	}

	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8","NO");
	poDataset=(GDALDataset*)GDALOpen(lpszPathName,GA_ReadOnly);
	if (poDataset==NULL)
	{
		AfxMessageBox("打开栅格数据集失败");
		return FALSE;
	}
	m_nbands=poDataset->GetRasterCount();
	pbands=new GDALRasterBand *[m_nbands];  //申请波段指针数组？
	if (pbands==NULL)
	{
		AfxMessageBox("获得波段数据集失败");
		return FALSE;
	}
	for (int i=0;i<m_nbands;i++)
	{
		pbands[i]=poDataset->GetRasterBand(i+1);
	}
	//获取图像的宽度和高度
	m_nWidth=pbands[0]->GetXSize();
	m_nHeigth=pbands[0]->GetYSize();
	if (m_nbands==1)
	{
		for (int i=0;i<3;i++)
		{
			pBandBlock[i]=(BYTE*)CPLMalloc(sizeof(BYTE)*m_nHeigth*m_nWidth);
			pbands[0]->RasterIO(GF_Read,0,0,m_nWidth,m_nHeigth,pBandBlock[i],
				m_nWidth,m_nHeigth,GDT_Byte,0,0);
		}
	}
	if (m_nbands!=1)
	{
		for (int i=0;i<m_nbands;i++)
		{
			pBandBlock[i]=(BYTE*)CPLMalloc(sizeof(BYTE)*m_nHeigth*m_nWidth);
			pbands[i]->RasterIO(GF_Read,0,0,m_nWidth,m_nHeigth,pBandBlock[i],
				m_nWidth,m_nHeigth,GDT_Byte,0,0);
		}	
	}
	Calculate();
	UpdateAllViews(NULL);
	return TRUE;
}
/************************************************************************/
/* 统计灰度信息，用于直方图显示                                                  */
/************************************************************************/
void CRsProcDoc::Calculate()
{
	for (int i=0;i<m_nHeigth;i++)
	{
		for (int j=0;j<m_nWidth;j++)
		{
			count[0][pBandBlock[0][i*m_nWidth+j]]++;
			count[1][pBandBlock[1][i*m_nWidth+j]]++;
			count[2][pBandBlock[2][i*m_nWidth+j]]++;

			if (pBandBlock[0][i*m_nWidth+j]>max_grey[0])
			{
				max_grey[0]=pBandBlock[0][i*m_nWidth+j];
			}
			if (pBandBlock[1][i*m_nWidth+j]>max_grey[1])
			{
				max_grey[1]=pBandBlock[1][i*m_nWidth+j];
			}
			if (pBandBlock[2][i*m_nWidth+j]>max_grey[2])
			{
				max_grey[2]=pBandBlock[2][i*m_nWidth+j];
			}
			if (pBandBlock[0][i*m_nWidth+j]<min_grey[0])
			{
				min_grey[0]=pBandBlock[0][i*m_nWidth+j];
			}
			if (pBandBlock[1][i*m_nWidth+j]<min_grey[1])
			{
				min_grey[1]=pBandBlock[1][i*m_nWidth+j];
			}
			if (pBandBlock[2][i*m_nWidth+j]<min_grey[2])
			{
				min_grey[2]=pBandBlock[2][i*m_nWidth+j];
			}
		}
	}
	for (int k=0;k<256;k++)
	{
		if (count[0][k]>max[0])
		{
			max[0]=count[0][k];
		}
		if (count[1][k]>max[1])
		{
			max[1]=count[1][k];
		}
		if (count[2][k]>max[2])
		{
			max[2]=count[2][k];
		}
	}
}
void CRsProcDoc::OnHistShow() 
{
	// TODO: Add your command handler code here	
	CHistDlg *pdlg=new CHistDlg();
	pdlg->Create(IDD_HIST_DLG);
	pdlg->ShowWindow(SW_SHOW);
	pdlg->CenterWindow();
}


void CRsProcDoc::OnFileSaveAs()
{
	// TODO: 在此添加命令处理程序代码
	const char* szFilter="BMP Files(*.BMP)|*.bmp|GeoTif Files(*.tif)|*.tif|IMG File(*.img)|*.img||";
	CFileDialog filedlg(false,"bmp",NULL,OFN_OVERWRITEPROMPT,szFilter);
	const char *pszFormat=NULL;
	CString FileSaveAsPathName;
	if (filedlg.DoModal()==IDOK)
	{
		CString ext=filedlg.GetFileExt();
		ext.MakeLower();//转换为小写
		if (ext=="bmp")
			pszFormat="BMP";
		else if (ext=="tif")
			pszFormat="GTiff";
		else if (ext=="img")
			pszFormat="HFA";
		else if (ext=="jpg")
			pszFormat="JPEG";
		else
			pszFormat="GTiff";
		FileSaveAsPathName=filedlg.GetPathName();
	}
	else
		return;
	GDALDataset *poDsDataSet;
	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8","NO");
	GDALDriver *poDriver;
	poDriver=GetGDALDriverManager()->GetDriverByName(pszFormat);
	if (poDriver==NULL)
	{
		AfxMessageBox("输出文件格式不支持!");
		return;
	}

	poDsDataSet=poDriver->Create(FileSaveAsPathName,m_nWidth,m_nHeigth,
		m_nbands,pbands[0]->GetRasterDataType(),NULL);

	if (poDsDataSet==NULL)
	{
		AfxMessageBox("创建目标文件失败!");
		return;
	}
	for (int i=0;i<m_nbands;i++)
	{
		GDALRasterBand *poband;
		poband=poDsDataSet->GetRasterBand(i+1);
		poband->RasterIO(GF_Write,0,0,m_nWidth,m_nHeigth,pBandBlock[i],
			m_nWidth,m_nHeigth,pbands[i]->GetRasterDataType(),0,0);
	}
	GDALClose((GDALDatasetH)poDsDataSet);
}
/************************************************************************/
/*  函数1: CopyScreenToBitmap //将屏幕指定区域存成图片                                                                    */
/************************************************************************/
HBITMAP CRsProcDoc::CopyScreenToBitmap(LPRECT lpRect) //lpRect 代表选定区域
{
	HDC hScrDC, hMemDC;      
	// 屏幕和内存设备描述表
	HBITMAP hBitmap,hOldBitmap;   
	// 位图句柄
	int       nX, nY, nX2, nY2;      
	// 选定区域坐标
	int       nWidth, nHeight;      
	// 位图宽度和高度
	int       xScrn, yScrn;         
	// 屏幕分辨率
	// 确保选定区域不为空矩形
	if (IsRectEmpty(lpRect))
		return NULL;
	//为屏幕创建设备描述表
	hScrDC = CreateDC("DISPLAY", NULL, NULL, NULL);
	//为屏幕设备描述表创建兼容的内存设备描述表
	hMemDC = CreateCompatibleDC(hScrDC);
	// 获得选定区域坐标
	nX = lpRect->left;
	nY = lpRect->top;
	nX2 = lpRect->right;
	nY2 = lpRect->bottom;
	// 获得屏幕分辨率
	xScrn = GetDeviceCaps(hScrDC, HORZRES);
	yScrn = GetDeviceCaps(hScrDC, VERTRES);
	//确保选定区域是可见的
	if (nX < 0)
		nX = 0;
	if (nY < 0)
		nY = 0;
	if (nX2 > xScrn)
		nX2 = xScrn;
	if (nY2 > yScrn)
		nY2 = yScrn;
	nWidth = nX2 - nX;
	nHeight = nY2 - nY;
	// 创建一个与屏幕设备描述表兼容的位图
	hBitmap=CreateCompatibleBitmap(hScrDC,nWidth,nHeight);
	// 把新位图选到内存设备描述表中
	hOldBitmap=(HBITMAP)SelectObject(hMemDC,hBitmap);
	// 把屏幕设备描述表拷贝到内存设备描述表中
	BitBlt(hMemDC,0,0, nWidth,nHeight,hScrDC, nX, nY, SRCCOPY);
	//得到屏幕位图的句柄
	hBitmap=(HBITMAP)SelectObject(hMemDC,hOldBitmap);
	//清除 
	DeleteDC(hScrDC);
	DeleteDC(hMemDC);
	// 返回位图句柄
	return hBitmap;
}
/************************************************************************/
/*   函数2 : SaveBitmapToFile 将图片存成文件                                                                    */
/************************************************************************/
int CRsProcDoc::SaveBitmapToFile(HBITMAP hBitmap, LPSTR lpFileName) //hBitmap 为刚才的屏幕位图句柄
{      //lpFileName 为位图文件名
	HDC     hDC;         
	//设备描述表
	int     iBits;      
	//当前显示分辨率下每个像素所占字节数
	WORD    wBitCount;   
	//位图中每个像素所占字节数
	//定义调色板大小， 位图中像素字节大小 ，  位图文件大小 ， 写入文件字节数
	DWORD           dwPaletteSize=0,dwBmBitsSize,dwDIBSize, dwWritten;
	BITMAP          Bitmap;        
	//位图属性结构
	BITMAPFILEHEADER   bmfHdr;        
	//位图文件头结构
	BITMAPINFOHEADER   bi;            
	//位图信息头结构 
	LPBITMAPINFOHEADER lpbi;          
	//指向位图信息头结构
	HANDLE          fh, hDib, hPal;
	HPALETTE     hOldPal=NULL;
	//定义文件，分配内存句柄，调色板句柄

	//计算位图文件每个像素所占字节数
	hDC = CreateDC("DISPLAY",NULL,NULL,NULL);
	iBits = GetDeviceCaps(hDC, BITSPIXEL) * 
		GetDeviceCaps(hDC, PLANES);
	DeleteDC(hDC);
	if (iBits <= 1)
		wBitCount = 1;
	else if (iBits <= 4)
		wBitCount = 4;
	else if (iBits <= 8)
		wBitCount = 8;
	else if (iBits <= 24)
		wBitCount = 24;
	else
		wBitCount = 32;
	//计算调色板大小
	if (wBitCount <= 8)
		dwPaletteSize=(1<<wBitCount)*sizeof(RGBQUAD);

	//设置位图信息头结构
	GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&Bitmap);
	bi.biSize            = sizeof(BITMAPINFOHEADER);
	bi.biWidth           = Bitmap.bmWidth;
	bi.biHeight          = Bitmap.bmHeight;
	bi.biPlanes          = 1;
	bi.biBitCount         = wBitCount;
	bi.biCompression      = BI_RGB;
	bi.biSizeImage        = 0;
	bi.biXPelsPerMeter     = 0;
	bi.biYPelsPerMeter     = 0;
	bi.biClrUsed         = 0;
	bi.biClrImportant      = 0;

	dwBmBitsSize = ((Bitmap.bmWidth*wBitCount+31)/32)*4*Bitmap.bmHeight;
	//为位图内容分配内存

	/*xxxxxxxx计算位图大小分解一下(解释一下上面的语句)xxxxxxxxxxxxxxxxxxxx 
	//每个扫描行所占的字节数应该为4的整数倍，具体算法为:
	int biWidth = (Bitmap.bmWidth*wBitCount) / 32;
	if((Bitmap.bmWidth*wBitCount) % 32)
	biWidth++; //不是整数倍的加1
	biWidth *= 4;//到这里，计算得到的为每个扫描行的字节数。
	dwBmBitsSize = biWidth * Bitmap.bmHeight;//得到大小
	xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx*/


	hDib  = GlobalAlloc(GHND,dwBmBitsSize+dwPaletteSize+sizeof(BITMAPINFOHEADER));
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
	*lpbi = bi;
	// 处理调色板   
	hPal = GetStockObject(DEFAULT_PALETTE);
	if (hPal)
	{
		hDC = ::GetDC(NULL);
		hOldPal=SelectPalette(hDC,(HPALETTE)hPal,FALSE);
		RealizePalette(hDC);
	}
	// 获取该调色板下新的像素值
	GetDIBits(hDC,hBitmap,0,(UINT)Bitmap.bmHeight,(LPSTR)lpbi+sizeof(BITMAPINFOHEADER)+dwPaletteSize, (BITMAPINFO *)lpbi,DIB_RGB_COLORS);
	//恢复调色板   
	if (hOldPal)
	{
		SelectPalette(hDC, hOldPal, TRUE);
		RealizePalette(hDC);
		::ReleaseDC(NULL, hDC);
	}
	//创建位图文件    
	fh=CreateFile(lpFileName, GENERIC_WRITE,0, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (fh==INVALID_HANDLE_VALUE)
		return FALSE;
	// 设置位图文件头
	bmfHdr.bfType = 0x4D42;  // "BM"
	dwDIBSize=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+dwPaletteSize+dwBmBitsSize;  
	bmfHdr.bfSize = dwDIBSize;
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER)+(DWORD)sizeof(BITMAPINFOHEADER)+dwPaletteSize;
	// 写入位图文件头
	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
	// 写入位图文件其余内容
	WriteFile(fh, (LPSTR)lpbi, sizeof(BITMAPINFOHEADER)+dwPaletteSize+dwBmBitsSize , &dwWritten, NULL); 
	//清除   
	GlobalUnlock(hDib);
	GlobalFree(hDib);
	CloseHandle(fh);
	return TRUE;
}
void CRsProcDoc::OnFileSave()
{
	// TODO: 在此添加命令处理程序代码
}


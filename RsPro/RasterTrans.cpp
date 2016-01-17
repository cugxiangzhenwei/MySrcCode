//#include "RasterTrans.h"
//#include "ProgressWnd.h"
#include "StdAfx.h"
int RasterTrans(const char *pszInFile,const char *pszOutFile,const char *pszFormat,CProgressWnd *pro)
{
	GDALDataset *poSrcDataSet=NULL;
	GDALDataset *poDesDataSet=NULL;
	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8","NO");
	poSrcDataSet=(GDALDataset*)GDALOpen(pszInFile,GA_ReadOnly);
	if (poSrcDataSet==NULL)
	{
		return -1;
	}
	if (pro!=NULL)
	{
		pro->SetText("正在进行栅格格式转换");
	}
	GDALDriver *poDriver=(GDALDriver*)GDALGetDriverByName(pszFormat);
	if (poDriver==NULL)
	{
		return -2;
	}
	char **papszOptions=NULL;

	poDesDataSet=poDriver->CreateCopy(pszOutFile,poSrcDataSet,0,papszOptions,NULL,pro);

	const char * pszProRef=poSrcDataSet->GetProjectionRef();
	poDesDataSet->SetProjection(pszProRef);

	GDALClose((GDALDatasetH)poSrcDataSet);
	GDALClose((GDALDatasetH)poDesDataSet);
	delete pro;
	return 0;
}
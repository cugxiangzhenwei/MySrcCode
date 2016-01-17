/**********************************************************************
* @Name:     WarpALg.cpp
* @Project:  遥感图像处理工具
* @Purpose:  几何校正
* @Author:   向振伟
* @Copyright (c) 2011-12-4 cugxiangzhenwei@sina.cn 13488751151
**********************************************************************/
#include "ImgGeoWarpALg.h"
#include "Transform.h"
// #include "CoordinateTransform.h"
#include <gdal_priv.h>
#include <gdalwarper.h>
#include <gdal_alg.h>
#include <boost/progress.hpp>
using namespace boost;

#pragma warning(disable:4700 4018 )
CImgGeoWarp::CImgGeoWarp(const char *pszInFile,const char *pszOutFile,vector<GDAL_GCP>vGcp, const char *pszFormat,CProcessBase *pro)
{
	m_pszInFile=pszInFile;
	m_pszOutFile=pszOutFile;
	m_VGcpList=vGcp;
	m_pszFormat=pszFormat;
	m_progress=pro;
	m_pszWkt=NULL;
}
bool CImgGeoWarp::SetProjection(const char *pszPrj,bool bIsWkt)
{
	if (bIsWkt && pszPrj!=NULL && pszPrj!="")
	{
		m_pszWkt=pszPrj;
		return true;
	}
	else if (!bIsWkt)
	{
		GDALDatasetH hDs;
		GDALAllRegister();
		CPLSetConfigOption("GDAL_FILENAME_IS_UTF8","NO");
		hDs=GDALOpen(pszPrj,GA_ReadOnly);
		if (hDs==NULL)
			return false;
		const char *pszWtk=GDALGetProjectionRef(hDs);
		if (pszWtk==NULL)
			return false;
		else
			m_pszWkt=pszWtk;
		return true;
	}
	else
		return false;
}

bool CImgGeoWarp::InitTransform(GeoWarpMode warpMode,GDALTransformerFunc &hfnTransform,void **pPTransformerArg,GDAL_GCP * pGcpList,int iGcpCount)
{

	void *pTransformerArg = NULL;
	int iOder = 1;
	switch (warpMode)
	{
	case GTP_Polynomial_1:
	case GTP_Polynomial_2:
	case GTP_Polynomial_3:
		{
			for ( int i=0;i<iGcpCount;i++)
				pGcpList[i].dfGCPLine *= -1.0;
			
			iOder = int(warpMode) +1;
			hfnTransform = GDALGCPTransform;
			pTransformerArg =GDALCreateGCPTransformer(iGcpCount,(GDAL_GCP *)pGcpList,iOder,FALSE);
			break;

		}
	case GTP_ThinPlateSpline:
		{
			for ( int i=0;i<iGcpCount;i++)
				pGcpList[i].dfGCPLine *= -1.0;

			hfnTransform = GDALTPSTransform;
			pTransformerArg =GDALCreateTPSTransformer(iGcpCount,(GDAL_GCP *)pGcpList,FALSE);
			break;
		}
	case GTP_LinerTrans:
		{
			hfnTransform = LinerTransform;
			pTransformerArg =CreateLinerTransformer(iGcpCount,pGcpList);
			break;
		}
	case GTP_HelmertTrans:
		{
			hfnTransform = HelmertTransform;
			pTransformerArg =CreateHelmertTransformer(iGcpCount,pGcpList);
			break;
		}
	case GTP_ExteriorTrans:
		{
			hfnTransform = ExteriorOtreationTransform;
			pTransformerArg =CreateExteriorOtreationTransformArg(iGcpCount,pGcpList);
			break;
		}
	default:
		{
			hfnTransform = GDALGCPTransform;
			pTransformerArg =GDALCreateGCPTransformer(iGcpCount,(GDAL_GCP *)pGcpList,1,FALSE);
			break;
		}
	}
	if(!pTransformerArg)
		return false;
	else
		*pPTransformerArg = pTransformerArg;
	return true;
}

void CImgGeoWarp::DestroyTransformerArg(void ** ppTransforArg,GeoWarpMode warpMode)
{
	switch (warpMode)
	{
	case GTP_Polynomial_1:
	case GTP_Polynomial_2:
	case GTP_Polynomial_3:
		{

			GDALDestroyGCPTransformer(*ppTransforArg);
			break;
		}
	case GTP_ThinPlateSpline:
		{
			GDALDestroyTPSTransformer(*ppTransforArg);
			break;
		}
	case GTP_LinerTrans:
		{
			DestroyLinerTransformArg(*ppTransforArg);
			break;
		}
	case GTP_HelmertTrans:
		{
			DestroyHelmertTransformArg(*ppTransforArg);
			break;
		}
	case GTP_ExteriorTrans:
		{
			DestroyExteriorOtreationTransformArg(*ppTransforArg);
			break;
		}
	default:
		{
			GDALDestroyGCPTransformer(*ppTransforArg);
			break;
		}
	}
	*ppTransforArg = NULL;
}
int	CImgGeoWarp::DoExcuteWarp(GeoWarpMode warpMode,double dReX,double dReY,ResampleMethod Remethod)

{
	if (m_progress!=NULL)
	{
		m_progress->ReSetProcess();
		m_progress->SetMessage("开始执行几何纠正");
	}
	GDALDatasetH hSrcDs;
	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8","NO");

	if (m_pszInFile==NULL||m_pszOutFile==NULL)
		return RE_PARAMERROR;
	if (FileIsUsed(m_pszOutFile,m_progress))
		return RE_FAILED;
	if (m_pszWkt==NULL||m_pszWkt=="")
		return RE_PARAMERROR;

	int iGcpCount=m_VGcpList.size();

	if (iGcpCount==0)
		return RE_PARAMERROR;

	hSrcDs=GDALOpen(m_pszInFile,GA_ReadOnly);
	if (hSrcDs==NULL)
	{
		if (m_progress!=NULL)
			m_progress->SetMessage("打开原始影像失败!");
		return RE_FAILED ;
	}

	progress_timer *pTimer=new progress_timer;//开始计时

	double adfGeoTransform[6]={0};
	GDALGetGeoTransform(hSrcDs,adfGeoTransform);

	GDAL_GCP *pGcps=new GDAL_GCP[iGcpCount];
	for (int i=0;i<iGcpCount;i++)
	{
		pGcps[i]=m_VGcpList[i];
		pGcps[i].dfGCPPixel = m_VGcpList[i].dfGCPPixel;
		pGcps[i].dfGCPLine  = m_VGcpList[i].dfGCPLine;

		if (adfGeoTransform[1] ==1 && adfGeoTransform[5]==1)
			pGcps[i].dfGCPLine  *= -1;
	}

	GDALTransformerFunc hfnTransform;
	void *pTransformerArg = NULL;
	InitTransform(warpMode,hfnTransform,&pTransformerArg,pGcps,iGcpCount);
	RELEASE(pGcps);

	if (pTransformerArg==NULL)
	{
		if (m_progress!=NULL)
		{
			m_progress->SetMessage("计算转换关系失败,可能是控制点个数不足");
		}
		delete pTimer;
		GDALClose(hSrcDs);
		return RE_FAILED;
	}

	// 求得预测的四支范围和分辨率
	double adfDstGeoTransform[6]={0};
	int nPixles=0, nLines=0;

	CPLErr eErr = GDALSuggestedWarpOutput(hSrcDs, 
		hfnTransform, pTransformerArg, 
		adfDstGeoTransform, &nPixles, &nLines );
	if (eErr!=CE_None)
	{
		if (m_progress!=NULL)
		{
			m_progress->SetMessage("计算输出图像范围出错");
		}
		delete pTimer;
		GDALClose(hSrcDs);
		return RE_FAILED;
	}
	//如果用户指定了分辨率
	if(dReX!=0.0 || dReY!=0.0)
	{
		if (dReX==0.0) dReX=adfDstGeoTransform[1];
		if (dReY==0.0) dReY=adfDstGeoTransform[5];

		double Minx=adfDstGeoTransform[0];
		double Maxx=adfDstGeoTransform[0]+nPixles*adfDstGeoTransform[1];
		double Maxy=adfDstGeoTransform[3];
		double Miny=adfDstGeoTransform[3]+nLines*adfDstGeoTransform[5];

		nPixles= static_cast<int>((Maxx-Minx)/dReX+0.5);
		nLines = static_cast<int>((Maxy-Miny)/dReY+0.5);
		adfDstGeoTransform[0]=Minx;
		adfDstGeoTransform[1]=dReX;
		adfDstGeoTransform[3]=Maxy;
		adfDstGeoTransform[5]= - dReY;
	}
	//创建输出影像
	GDALDriverH hDriver=GDALGetDriverByName(m_pszFormat);
	GDALDataType eDT = GDALGetRasterDataType(GDALGetRasterBand(hSrcDs,1));
	int nBandCount=GDALGetRasterCount(hSrcDs);
	if (hDriver==NULL)
	{
		if (m_progress!=NULL)
		{
			m_progress->SetMessage("注册驱动程序失败，可能是输出格式不支持");
		}
		delete pTimer;
		GDALClose(hSrcDs);
		return RE_FILENOTSUPPORT;
	}
	GDALDatasetH hDstDs = GDALCreate( hDriver, m_pszOutFile, nPixles, nLines, nBandCount, eDT, NULL );
	if (hDstDs==NULL)
	{
		if (m_progress!=NULL)
		{
			m_progress->SetMessage("创建输出图像失败");
		}
		delete pTimer;
		GDALClose(hSrcDs);return RE_CREATEFAILED;

	}

	GDALSetProjection ( hDstDs, m_pszWkt);
	GDALSetGeoTransform( hDstDs, adfDstGeoTransform );
	//CPLFree((void*)m_pszWkt);
	for (int i=0;i<nBandCount;i++)
	{
		GDALColorTableH hCT=GDALGetRasterColorTable(GDALGetRasterBand(hSrcDs,i+1));
		if (hCT!=NULL)
			GDALSetRasterColorTable(GDALGetRasterBand(hSrcDs,i+1),hCT);
	}

	//确定输出影像的 warpoption的选项
	GDALWarpOptions *psWarpOptions = GDALCreateWarpOptions();
	psWarpOptions->hSrcDS = hSrcDs;
	psWarpOptions->hDstDS = hDstDs;
	psWarpOptions->nBandCount =nBandCount;
	psWarpOptions->panSrcBands =(int *) CPLMalloc(sizeof(int)*psWarpOptions->nBandCount);
	psWarpOptions->panDstBands =(int *) CPLMalloc(sizeof(int)*psWarpOptions->nBandCount);

	for (int i=0;i<nBandCount;i++)
	{
		psWarpOptions->panSrcBands[i]=i+1;
		psWarpOptions->panDstBands[i]=i+1;
	}

	psWarpOptions->dfWarpMemoryLimit=50*1024*1024;//限制转换使用的内存为50兆
	psWarpOptions->eResampleAlg = (GDALResampleAlg)Remethod;
	psWarpOptions->pfnProgress = ALGTermProgress;
	psWarpOptions->pProgressArg=m_progress;

	psWarpOptions->pfnTransformer = GeoToPixelTransform;
	psWarpOptions->pTransformerArg = CreateGeoToPixelTransform(hfnTransform,pTransformerArg,adfDstGeoTransform);

	// Initialize and execute the warp operation. 
	GDALWarpOperation oOperation;
	eErr =  oOperation.Initialize( psWarpOptions );
	CPLAssert(eErr==CE_None);
	eErr = oOperation.ChunkAndWarpImage( 0,0, nPixles, nLines);

	DestroyTransformerArg(&pTransformerArg,warpMode);
	DestroyGeoToPixelTransformArg(psWarpOptions->pTransformerArg );
	GDALDestroyWarpOptions( psWarpOptions );

	GDALClose(hDstDs);
	GDALClose(hSrcDs);

	if (eErr!=CE_None)
	{
		if (m_progress!=NULL)
		{
			m_progress->SetMessage("图像校正失败");
		}
		delete pTimer;
		remove(m_pszOutFile);
		return RE_FAILED;
	}

	if (m_progress!=NULL)
	{
		m_progress->SetMessage("几何校正成功!");
	}
	delete pTimer;
	return RE_SUCCESS;
}
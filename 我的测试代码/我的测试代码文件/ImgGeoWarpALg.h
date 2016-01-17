#ifndef _WARPALG_H
#define _WARPALG_H
#include "ImgCore.h"
#include "gdal_alg.h"
#include "gdal.h"
/**
* @brief 根据控制点做几何校正
*/
class  CImgGeoWarp
{
public:
	enum GeoWarpMode{
		GTP_Polynomial_1 = 0,
		GTP_Polynomial_2 = 1,
		GTP_Polynomial_3 = 2,
		GTP_ThinPlateSpline = 3,
		GTP_LinerTrans = 4,
		GTP_HelmertTrans = 5,
		GTP_ExteriorTrans = 6,
		GTP_InValid = 7
	};
public:
	CImgGeoWarp(const char *pszInFile,const char *pszOutFile,vector<GDAL_GCP>vGcp, const char *pszFormat,CProcessBase *pro);
	int DoExcuteWarp(GeoWarpMode warpMode =  GTP_Polynomial_1,double dReX=0.0,double dReY=0.0,ResampleMethod Remethod=RM_Bilinear) ;
	bool SetProjection(const char *pszPrj,bool bIsWkt=true);
private:
	bool  InitTransform(CImgGeoWarp::GeoWarpMode warpMode,GDALTransformerFunc &hfnTransform,void **pPTransformerArg,GDAL_GCP * pGcpList,int iGcpCount);
	void DestroyTransformerArg(void **	pPTransforArg,GeoWarpMode warpMode);
	const char* m_pszInFile;
	const char* m_pszOutFile;
	vector<GDAL_GCP> m_VGcpList;
	const char* m_pszFormat;
	const char* m_pszWkt;
	CProcessBase *m_progress;
};
#endif // _WARPALG_H
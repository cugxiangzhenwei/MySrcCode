#include "Transform.h"
#include "MartixAlgo.h"
#include <limits>
#include <gdal.h>

void *CreateExteriorOtreationTransformArg(int iGcpCout,GDAL_GCP * pGcpList)
{
	const char *pszKTK = "PROJCS[\"Transverse Mercator\",GEOGCS[\"Krasovsky\",DATUM[\"Krasovsky\",SPHEROID[\"Krasovsky\",6378245,298.3000003760163],\
						 TOWGS84[0,0,0,0,0,0,0]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.0174532925199433]],PROJECTION[\"Transverse_Mercator\"],ARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",117],PARAMETER[\"scale_factor\",1],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",0],UNIT[\"meters\",1]]";

	vector <GDAL_GCP> vGcp;
	for (int i=0;i<iGcpCout;i++)
	{
		vGcp.push_back(pGcpList[i]);
	}

	CPushbroomCameraModel *mode = new CPushbroomCameraModel(580, 0.0065,0.0065,0.0, 0.0, 2.90888208656e-5);
	mode->SetImageSize(24000,24000);
	mode->SetMinMaxElevation(0,1150);
	mode->SetSatAngle(830000, -3.879108);
	mode->SetGCPs(vGcp,pszKTK);
	mode->SolveExteriorOrientationMine(10);

	SExteriorOrientationTransformArg * extArg = new SExteriorOrientationTransformArg;
	memcpy(&extArg->m_exterior,mode->GetExterior(),sizeof(SExteriorOrientation));
	extArg->mPushMode = mode;
	return extArg;
}
void DestroyExteriorOtreationTransformArg(void * Arg)
{
	SExteriorOrientationTransformArg* pArg = static_cast<SExteriorOrientationTransformArg*>(Arg);
	delete pArg->mPushMode;
	delete pArg;
	Arg = NULL;
}
int  ExteriorOtreationTransform(void *pTransformArg, int bDstToSrc, int nPointCount,double *x, double *y, double *z, int *panSuccess )
{
	SExteriorOrientationTransformArg * exteriorArg = static_cast<SExteriorOrientationTransformArg*>(pTransformArg);
	CPushbroomCameraModel *mode = exteriorArg->mPushMode;

	// 相机焦距
	double dFocalLength = mode->GetFocalLength();

	double dXS = 0, dYS = 0, dZS = 0;
	double a11 = 1, a12 = 0, a13 = 0;
	double a21 = 0, a22 = 1, a23 = 0;
	double a31 = 0, a32 = 0, a33 = 1;

	if (panSuccess != NULL)
		memset(panSuccess, TRUE, sizeof(int)*nPointCount);

	if (bDstToSrc) //反算，用于重采样
	{
		double dCol,dRow;
		for (int i=0;i<nPointCount;i++)
		{
			mode->InterateGetRowCol(x[i],y[i],z[i],dRow,dCol,50);
			x[i] = dCol;
			y[i] = dRow;
		}
	}
	else //正算，用于计算输出图像的范围
	{
		for (int i=0;i<nPointCount;i++)
		{
			x[i] = (x[i]- mode->GetImageWidth()/2)*mode->GetPixelXSize()/1000;
			double dDelta = (mode->GetImageHeight()/2 - y[i])*mode->GetPixelYSize()/1000;
			double dTmpx = x[i]; 
			x[i] = mode->m_dAffineTransform[0] + mode->m_dAffineTransform[1]*dTmpx + mode->m_dAffineTransform[2]*dDelta;
			y[i] = mode->m_dAffineTransform[3] + mode->m_dAffineTransform[4]*dTmpx + mode->m_dAffineTransform[5]*dDelta;
			//y[i] = exteriorArg->m_exterior.s_dYs + exteriorArg->m_exterior.s_dCoeffYs * dDelta;
			z[i] = 0; //地面高程Z
		}
	}
	return true;
}
int  LinerTransform(void *pTransformArg, int bDstToSrc, int nPointCount,double *x, double *y, double *z, int *panSuccess )
{
	LinerTransformParam * lt = static_cast<LinerTransformParam*>(pTransformArg);
	if (!lt)
		return false;
	if (!bDstToSrc)
	{
		for (int i=0;i<nPointCount;i++)
		{
			x[i]  = x[i] * lt->m_dScaleX + lt->orgin.x;
			y[i]  = -y[i] * lt->m_dScaleY + lt->orgin.y;
			if (panSuccess)	panSuccess[i] = true; 
		}

	}
	else
	{
		if (fabs( lt->m_dScaleX ) < std::numeric_limits<double>::epsilon() || 
			fabs( lt->m_dScaleY ) < std::numeric_limits<double>::epsilon() )
		{
			if (panSuccess)
			{
				for ( int i = 0; i < nPointCount; ++i )
					panSuccess[i] = false;
			}

			return false;
		}
		for (int i=0;i<nPointCount;i++)
		{
			x[i] = (x[i] - lt->orgin.x) / lt->m_dScaleX;
			y[i] = (y[i] - lt->orgin.y) / (- lt->m_dScaleY);
			if (panSuccess)	panSuccess[i] = true; 
		}
	}

	return true;
}
void*  CreateLinerTransformer(int iGcpCout,GDAL_GCP * pGcpList)
{
	if (iGcpCout<2 || !pGcpList)
		return NULL;

	LinerTransformParam  * lt = new LinerTransformParam;

	double dSumPx(0.0),dSumPy(0.0),dSumPx2(0.0),dSumPy2(0.0),dSumMx(0.0),dSumMy(0.0),dSumPxMx(0.0),dSumPyMy(0.0);

	for ( int i=0;i<iGcpCout;i++)
	{
		dSumPx += pGcpList[i].dfGCPPixel;
		dSumPy += pGcpList[i].dfGCPLine;
		dSumPx2 += pow(pGcpList[i].dfGCPPixel,2); 
		dSumPy2 += pow(pGcpList[i].dfGCPLine,2); 

		dSumMx += pGcpList[i].dfGCPX;
		dSumMy += pGcpList[i].dfGCPY;

		dSumPxMx += pGcpList[i].dfGCPPixel * pGcpList[i].dfGCPX;
		dSumPyMy += pGcpList[i].dfGCPLine  * pGcpList[i].dfGCPY;
	}

	double deltaX = iGcpCout * dSumPx2 - pow( dSumPx, 2 );
	double deltaY = iGcpCout * dSumPy2 - pow( dSumPy, 2 );

	lt->orgin.x = ( dSumPx2 * dSumMx - dSumPx * dSumPxMx ) / deltaX;  //计算中心点X坐标
	lt->orgin.y = ( dSumPy2 * dSumMy - dSumPy * dSumPyMy ) / deltaY;  //计算中心点Y坐标

	double bX = ( iGcpCout * dSumPxMx - dSumPx * dSumMx ) / deltaX; // 计算X方向缩放比
	double bY = ( iGcpCout * dSumPyMy - dSumPy * dSumMy ) / deltaY; // 计算Y方向缩放比

	lt->m_dScaleX = abs(bX);
	lt->m_dScaleY = abs(bY);

	return lt;
}

void  DestroyLinerTransformArg( void * TransFormArg)
{
	delete static_cast<LinerTransformParam*>(TransFormArg);
	TransFormArg  = NULL;
}

int  HelmertTransform(void *pTransformArg, int bDstToSrc, int nPointCount,double *x, double *y, double *z, int *panSuccess )
{
	HelmertParameters * ht = static_cast<HelmertParameters*>(pTransformArg);
	if (!ht)
		return false;

	double a = cos( ht ->angle ), b = sin( ht->angle ), x0 = ht->origin.x, y0 = ht->origin.y, s = ht->scale;

	if ( !bDstToSrc )
	{
		a *= s;
		b *= s;

		for ( int i = 0; i < nPointCount; ++i )
		{
			double xT = x[i], yT = y[i];
			// Because rotation parameters where estimated in a CS with negative y-axis ^= down.
			// we need to apply the rotation matrix and a change of base:
			// |cos a,-sin a| |1, 0|   | cos a,  sin a|
			// |sin a, cos a| |0,-1| = | sin a, -cos a|
			x[i] = x0 + ( a * xT + b * yT );
			y[i] = y0 + ( b * xT - a * yT );
			panSuccess[i] = true;
		}
	}
	else
	{
		// Guard against division by zero
		if ( abs( s ) < std::numeric_limits<double>::epsilon() )
		{
			for ( int i = 0; i < nPointCount; ++i )
			{
				panSuccess[i] = false;
			}
			return false;
		}

		a /= s;
		b /= s;

		for ( int i = 0; i < nPointCount; ++i )
		{
			double xT = x[i], yT = y[i];
			xT -= x0;
			yT -= y0;
			// | cos a,  sin a |^-1   |cos a,  sin a|
			// | sin a, -cos a |    = |sin a, -cos a|
			x[i] =  a * xT + b * yT;
			y[i] =  b * xT - a * yT;
			panSuccess[i] = true;
		}
	}
	return true;
}
void  * CreateHelmertTransformer(int iGcpCout,GDAL_GCP * pGcpList)
{
	if (iGcpCout<2 || !pGcpList)
		return NULL;

	HelmertParameters  * ht = new HelmertParameters;

	double A = 0.0, B = 0.0, C = 0.0, D = 0.0, E = 0.0, F = 0.0, G = 0.0, H = 0.0, I = 0.0, J = 0.0;

	for ( int i=0; i<iGcpCout; ++i )
	{
		A += pGcpList[i].dfGCPPixel;
		B += pGcpList[i].dfGCPLine;
		C +=  pGcpList[i].dfGCPX;
		D +=  pGcpList[i].dfGCPY;
		E += pGcpList[i].dfGCPX * pGcpList[i].dfGCPPixel;
		F += pGcpList[i].dfGCPY * pGcpList[i].dfGCPLine;
		G += pow( pGcpList[i].dfGCPPixel, 2 );
		H += pow( pGcpList[i].dfGCPLine, 2 );
		I += pGcpList[i].dfGCPX * pGcpList[i].dfGCPLine;
		J += pGcpList[i].dfGCPPixel *  pGcpList[i].dfGCPY;
	}

	/* The least squares fit for the parameters { a, b, x0, y0 } is the solution
	to the matrix equation Mx = b, where M and b is given below. I *think*
	that this is correct but I derived it myself late at night. Look at
	helmert.jpg if you suspect bugs. */

	double MData[] = 
	{
		A,   -B,    iGcpCout,    0,
		B,    A,    0,    iGcpCout,
		G+H,  0,    A,    B,
		0,    G+H, -B,    A
	};

	double bData[] = { C, D, E+F, J-I };

	// we want to solve the equation M*x = b, where x = [a b x0 y0]
	ExtMatrix eM(MData, 4, 4);
	ExtMatrix eB(bData, 4, 1);

	Matrix mM(4, 4);
	Matrix mB(4, 1);
	mtl::twod_copy_default(eM, mM);
	mtl::twod_copy_default(eB, mB);

	Vector X(4);

	bool f = SloveMartix(mM, mB, X);
	if (!f)
		return false;

	ht->origin.x = ( X[2] );
	ht->origin.y = ( X[3] );
	ht->scale = sqrt( pow( X[0], 2 ) + pow( X[1], 2 ) );
	ht->angle = atan2( X[1], X[0] );

	return ht;
}
void  DestroyHelmertTransformArg( void * TransFormArg)
{
	delete static_cast<HelmertParameters*>(TransFormArg);
	TransFormArg  = NULL;
}

void* CreateGcpPolynomailTransformArg(GDAL_GCP * pGcpList,int iGcpCount,int iPolynopmialOrder)
{
	return NULL;
}
void  DestroyGcpPolynomailArg(void * pTransformArg)
{
	GcpPolynomialPamram* pg = static_cast<GcpPolynomialPamram*>(pTransformArg);
	RELEASE(pg->m_dDstToSrcParam);
	RELEASE(pg->m_dSrcToDstParam);
	delete static_cast<GcpPolynomialPamram*>(pTransformArg);
	pTransformArg = NULL;
}
int GcpPolynomailTransform(void *pTransformArg, int bDstToSrc, int nPointCount,double *x, double *y, double *z, int *panSuccess )
{
	GcpPolynomialPamram * gp = static_cast<GcpPolynomialPamram*>(pTransformArg);
	if (!gp)
		return FALSE;

	double xi(0.0),yi(0.0); 
	int n = (gp->iPolynopmialOrder + 1) *(gp->iPolynopmialOrder +2);
	double * dParam = NULL;
	if (bDstToSrc)
		dParam = gp->m_dDstToSrcParam;
	else
		dParam = gp->m_dSrcToDstParam;

	if (gp->iPolynopmialOrder ==1)
	{
		for (int i=0;i<nPointCount;i++)
		{
			xi =  x[i];
			yi =  y[i];
			x[i]  = dParam[0] + dParam[1] *xi + dParam[2] * yi ;
			y[i]  = dParam[3] + dParam[4] *xi + dParam[5] * yi ;
			if (panSuccess)	panSuccess[i] = true; 
		}
	}
	else if (gp->iPolynopmialOrder ==2)
	{
		for (int i=0;i<nPointCount;i++)
		{
			xi =  x[i];
			yi =  y[i];
			x[i]  = dParam[0] + dParam[1] *xi + dParam[2] * yi  + dParam[3]*xi*xi  + dParam[4]*xi*yi  + dParam[5]*yi*yi;
			y[i]  = dParam[6] + dParam[7] *xi + dParam[8] * yi  + dParam[9]*xi*xi  + dParam[10]*xi*yi + dParam[11]*yi*yi;
			if (panSuccess)	panSuccess[i] = true; 
		}
	}
	else
	{
		for (int i=0;i<nPointCount;i++)
		{
			xi =  x[i];
			yi =  y[i];
			x[i]  = dParam[0] + dParam[1]*xi + dParam[2]*yi  + dParam[3]*xi*xi +dParam[4]*xi*yi
				+ dParam[5]*yi*yi + dParam[6]*xi*xi*xi + dParam[7]*xi*xi*yi + dParam[8]*xi*yi*yi + dParam[9]*yi*yi*yi ;

			y[i]  = dParam[10] + dParam[11] *xi + dParam[12]*yi  + dParam[13]*xi*xi +dParam[14]*xi*yi 
				+ dParam[15]*yi*yi + dParam[16]*xi*xi*xi + dParam[17]*xi*xi*yi + dParam[18]*xi*yi*yi + dParam[19]*yi*yi*yi;
			if (panSuccess)	panSuccess[i] = true; 
		}
	}
	return TRUE;
}
int GeoToPixelTransform( void *pTransformerArg, int bDstToSrc, int nPointCount,double *x, double *y, double *z, int *panSuccess )
{
	GeoTransStruct*chain=(GeoTransStruct*)pTransformerArg;
	if (chain==NULL)
		return FALSE;
	if (bDstToSrc)
	{
		for(int i=0;i<nPointCount;i++)
		{
			double P=x[i];
			double L=y[i];
			x[i]=chain->adfGeoTransform[0] + P*chain->adfGeoTransform[1]+ L*chain->adfGeoTransform[2];
			y[i]=chain->adfGeoTransform[3]+  P*chain->adfGeoTransform[4]+ L*chain->adfGeoTransform[5];	
		}
		if (!chain->hfnTransform(chain->pTransformArg,bDstToSrc,nPointCount,x,y,z,panSuccess))
			return FALSE;
	}
	else
	{
		if (!chain->hfnTransform(chain->pTransformArg,bDstToSrc,nPointCount,x,y,z,panSuccess))
			return FALSE;
		for(int i=0;i<nPointCount;i++)
		{
			if (!panSuccess[i])
				continue;
			double P=x[i];
			double L=y[i];
			x[i]=chain->adfGeoTransform[0] + P*chain->adfGeoTransform[1]+ L*chain->adfGeoTransform[2];
			y[i]=chain->adfGeoTransform[3]+  P*chain->adfGeoTransform[4]+ L*chain->adfGeoTransform[5];
		}
	}
	return TRUE;
}
void* CreateGeoToPixelTransform(GDALTransformerFunc hfnTransform,void *pTransformArg,double *adfGeoTranform)
{
	GeoTransStruct *chain=new GeoTransStruct;
	chain->hfnTransform=hfnTransform;
	chain->pTransformArg=pTransformArg;
	memcpy(chain->adfGeoTransform,adfGeoTranform,sizeof(double)*6);
	if (!GDALInvGeoTransform(chain->adfGeoTransform,chain->adfIvtGeoTransform))
	{
		return NULL;
	}
	return (void*)chain;
}
void DestroyGeoToPixelTransformArg(void *pTransformArg)
{
	delete static_cast<GeoTransStruct*>(pTransformArg);
}

int ReSampleImage(GDALDatasetH hSrcDs,GDALDatasetH hDstDs,void *pTransFormArg,GDALTransformerFunc &hTransform,double dMapMinx,double dMapMiny,ResampleMethod resampleMethod,CProcessBase *pro)
{
	if (!hDstDs || !hSrcDs || !pTransFormArg || !hTransform)
		return RE_PARAMERROR;

	int iSrcWidth  = GDALGetRasterXSize(hSrcDs);
	int iSrcHeight = GDALGetRasterYSize(hSrcDs);
	int iSrcBand   =  GDALGetRasterCount(hSrcDs);

	int iDstWidth  = GDALGetRasterXSize(hDstDs);
	int iDstHeight = GDALGetRasterYSize(hDstDs);
	int iDstBand   =  GDALGetRasterCount(hDstDs);

	if (iDstBand > iSrcBand  || iDstBand <=0)
		return RE_PARAMERROR;

	int nBands = iDstBand;
	int * pBandMap = new int[nBands];
	for ( int i=0;i<nBands;i++)
		pBandMap[i] = nBands - i;

	int iRev = RE_SUCCESS;

	DT_8U *InData = new DT_8U[iSrcWidth * iSrcHeight * nBands];
	if (!InData)
		return RE_FAILED;

	CPLErr err = GDALDatasetRasterIO(hSrcDs, GF_Read, 0, 0,iSrcWidth,iSrcHeight, InData,iSrcWidth,iSrcHeight, GDT_Byte,nBands, pBandMap,sizeof(DT_8U)*nBands,sizeof(DT_8U)*nBands * iSrcWidth,sizeof(DT_8U));
	if (err!=CE_None)
		return RE_FAILED;

	DT_8U* OutData = new DT_8U[iDstWidth*iDstHeight*nBands];
	if (!OutData)
		return RE_FAILED;

	if (pro)
		pro->SetStepCount(iDstHeight);

	double z = 0;
	for (int i = 0; i < iDstHeight; i++)
	{
		for (int j = 0; j <iDstWidth; j++)
		{
			double x = j + dMapMinx ; //x 为地图坐标
			double y = (iDstHeight - i) + dMapMiny; //y 为地图坐标

			hTransform(pTransFormArg,TRUE,1,&x,&y,&z,NULL);

			int icol = (int)(x + 0.5);
			int irow = (int)(y + 0.5);
			double x_dis = x - icol;
			double y_dis = y - irow;

			if ( icol < 0  || icol >= iSrcWidth || irow < 0 || irow >=iSrcHeight  )
			{
				for (int k = 0;k<nBands;k++)
					OutData[i*iDstWidth*nBands + j*nBands + k] = DT_8U(0);
			}
			else
			{
				if (resampleMethod == RM_NearestNeighbour)
					for (int k = 0;k<nBands;k++)
						OutData[i*iDstWidth*nBands + j*nBands + k] = InData[irow*iSrcWidth*nBands + icol*nBands + k];

				else if (resampleMethod == RM_Bilinear)
					for (int k = 0;k<nBands;k++)
					{
						double Value  = InData[irow*iSrcWidth*nBands + icol*nBands+k] * (1 - x_dis) *(1- y_dis)  +
										InData[irow*iSrcWidth*nBands + (icol+1)*nBands+k] * (1-y_dis)*x_dis +
										InData[(irow+1)*iSrcWidth*nBands + icol*nBands+k] * y_dis*(1-x_dis) +
										InData[(irow+1)*iSrcWidth*nBands + (icol+1)*nBands+k] *x_dis * y_dis;
						OutData[i*iDstWidth*nBands + j*nBands + k] = (DT_8U)Value;
					}
			}
		}

		if (pro)
		{
			if (!pro->StepIt())
			{
				pro->SetMessage("用户取消!");
				RELEASE(InData);
				RELEASE(OutData);
				RELEASE(pBandMap);
				return RE_USERCANCEL;
			}
		}
	}                

	err = GDALDatasetRasterIO(hDstDs,GF_Write,0,0,iDstWidth,iDstHeight,OutData,
		iDstWidth,iDstHeight,GDT_Byte,nBands,pBandMap,sizeof(DT_8U)*nBands,sizeof(DT_8U)*nBands * iDstWidth,sizeof(DT_8U));
	if (err!=CE_None)
		iRev =  RE_FAILED;

	RELEASE(InData);
	RELEASE(OutData);
	RELEASE(pBandMap);

	return iRev;
}
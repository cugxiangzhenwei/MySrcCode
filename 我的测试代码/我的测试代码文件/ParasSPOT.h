#include "gdal_priv.h"
#include<vector>
#include <boost/date_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
using namespace std;
using namespace boost;
using namespace boost::posix_time;
using namespace boost::gregorian;

/*
*@breif 卫星位置结构体
*/
typedef struct  
{
	double dX;
	double dY;
	double dZ;

	double dVx;
	double dVy;
	double dVz;

	ptime ptm;

}SatellitePosition;
/*
*@breif 卫星姿态角结构体
*/
typedef struct  
{
	double dYAW;
	double dPITCH;
	double dROLL;

	ptime ptm;
}SatelliteAngle;
/*
*@breif 卫星最低点位置结构体
*/
typedef struct
{
	double dLat;
	double dLon;
	double dAltitude;
}NADIRPOS;

typedef struct
{
	int row;
	int col;
	ptime ptm;
}SensorCenter;

typedef GDAL_GCP FrameCenter;
typedef SatelliteAngle SatelliteAngleSpeed;
typedef double LookAngle; 

typedef struct
{
	double lsp;
	LookAngle AlongTrackAngle;
	LookAngle AcrossTrackAngle;
	vector<GDAL_GCP> vGcp;
	vector<SatellitePosition> vPos;
	vector<SatelliteAngle> vAngle;
	vector<SatelliteAngleSpeed> vAngleSpeed;
	NADIRPOS  nadirpos;
	SensorCenter sensorCenter;
	FrameCenter  frameCenter;
}SpotDimParam;

/*
*@brief 从SPOT元数据中获取成像时卫星轨道参数
*/
bool GetSpotDimParam(const char *pszMetaData,SpotDimParam & spotDimParam);
/*
*@brief 卫星成像时刻插值
*/
ptime InterpolationT(SensorCenter sCenter,int iLine,double dLsp);
/*
*@brief 卫星姿态角插值
*/
void  InterpolationSensorAngle(SatelliteAngle ang1,SatelliteAngle ang2,SatelliteAngle & outAng);
/*
*@brief 卫星位置及线速度插值
*/
void  InterpolationSensorPosition(SpotDimParam * DimPram,SatellitePosition  &SatellitePos);
/*
*@brief 卫星姿态角插值
*/
void  InterpolationSensorAngle(SpotDimParam * DimPram,SatelliteAngle & outAng);
/*
*@brief 卫星姿态角速度插值
*/
void  InterpolationAngleSpeed(SpotDimParam * DimPram,SatelliteAngleSpeed  &AngSpeed);

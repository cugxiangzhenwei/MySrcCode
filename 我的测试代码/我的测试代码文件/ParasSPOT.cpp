#include "ParasSPOT.h"
#include "ImgCore.h"

#include "gdal_priv.h"
#include "ogrsf_frmts.h"
#include "ogr_api.h"
#include "CoordinateTrans.h"

#include "boost/assign.hpp"
#include "boost/foreach.hpp"
#include "boost/filesystem.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/xml_parser.hpp"
#include "boost/xpressive/xpressive_dynamic.hpp"
#include "boost/algorithm/string.hpp"

using namespace boost;
using namespace boost::assign;
using namespace boost::property_tree;
using namespace boost::xpressive;


bool GetSpotDimParam(const char *pszMetaData,SpotDimParam & spotDimParam)
{
	SET_LOCAL;
	ptree pt;
	read_xml(pszMetaData, pt);

	BOOST_FOREACH(ptree::value_type &v,	pt.get_child("Dimap_Document.Dataset_Frame"))
	{
		if(v.first == "Vertex")
		{
			GDAL_GCP gcp;
			gcp.pszId =NULL;
			gcp.pszInfo=NULL;

			string strValue = v.second.get_optional<string>("FRAME_LON").get_value_or("0");
			gcp.dfGCPX= lexical_cast<double>(strValue); //经度

			strValue = v.second.get_optional<string>("FRAME_LAT").get_value_or("0");
			gcp.dfGCPY= lexical_cast<double>(strValue); //维度

			gcp.dfGCPZ =0; //高程Z

			strValue = v.second.get_optional<string>("FRAME_ROW").get_value_or("0");
			gcp.dfGCPLine= lexical_cast<double>(strValue); //行号

			strValue = v.second.get_optional<string>("FRAME_COL").get_value_or("0");
			gcp.dfGCPPixel= lexical_cast<double>(strValue); //列号

			spotDimParam.vGcp.push_back(gcp);
		}
	}

	string strCenter = pt.get<string>("Dimap_Document.Dataset_Frame.Scene_Center.FRAME_LON", "0");//框标中心经度
	spotDimParam.frameCenter.dfGCPX= lexical_cast<double>(strCenter); 
	strCenter = pt.get<string>("Dimap_Document.Dataset_Frame.Scene_Center.FRAME_LAT", "0");//框标中心纬度
	spotDimParam.frameCenter.dfGCPY= lexical_cast<double>(strCenter); 
	strCenter = pt.get<string>("Dimap_Document.Dataset_Frame.Scene_Center.FRAME_ROW", "0");//框标中心行号
	spotDimParam.frameCenter.dfGCPLine= lexical_cast<double>(strCenter); 
	strCenter = pt.get<string>("Dimap_Document.Dataset_Frame.Scene_Center.FRAME_COL", "0");//框标中心列号
	spotDimParam.frameCenter.dfGCPPixel= lexical_cast<double>(strCenter); 
	spotDimParam.frameCenter.pszId=NULL;
	spotDimParam.frameCenter.pszInfo=NULL;
	spotDimParam.frameCenter.dfGCPZ = 0;


	string lookalg = pt.get<string>("Dimap_Document.Dataset_Sources.Source_Information.Scene_Source.INCIDENCE_ANGLE", "0");//框标中心经度
	spotDimParam.AlongTrackAngle= lexical_cast<double>(lookalg); 
	lookalg = pt.get<string>("Dimap_Document.Dataset_Sources.Source_Information.Scene_Source.VIEWING_ANGLE", "0");//框标中心经度
	spotDimParam.AcrossTrackAngle= lexical_cast<double>(lookalg); 


	string strlsp = pt.get<string>("Dimap_Document.Data_Strip.Sensor_Configuration.Time_Stamp.LINE_PERIOD", "0");//扫描行周期
	spotDimParam.lsp = lexical_cast<double>(strlsp); 

	string strValue = pt.get<string>("Dimap_Document.Data_Strip.Ephemeris.SATELLITE_ALTITUDE", "0"); //底点高程
	spotDimParam.nadirpos.dAltitude = lexical_cast<double>(strValue); 

	strValue = pt.get<string>("Dimap_Document.Data_Strip.Ephemeris.NADIR_LAT", "0"); //底点纬度
	spotDimParam.nadirpos.dLat = lexical_cast<double>(strValue); 

	strValue = pt.get<string>("Dimap_Document.Data_Strip.Ephemeris.NADIR_LON", "0");//底点经度
	spotDimParam.nadirpos.dLon = lexical_cast<double>(strValue); 



	strValue = pt.get<string>("Dimap_Document.Data_Strip.Sensor_Configuration.Time_Stamp.SCENE_CENTER_TIME", "0");//传感器中心时刻
	replace_all(strValue, "T", " ");
	spotDimParam.sensorCenter.ptm  = time_from_string(strValue);

	strValue = pt.get<string>("Dimap_Document.Data_Strip.Sensor_Configuration.Time_Stamp.SCENE_CENTER_LINE", "0");//传感器中心行
	spotDimParam.sensorCenter.row = lexical_cast<int>(strValue); 

	strValue = pt.get<string>("Dimap_Document.Data_Strip.Sensor_Configuration.Time_Stamp.SCENE_CENTER_COL", "0");//传感器中心列
	spotDimParam.sensorCenter.col = lexical_cast<int>(strValue); 


	BOOST_FOREACH(ptree::value_type &v,	pt.get_child("Dimap_Document.Data_Strip.Ephemeris.Points")) //获取传感器位置,和对应位置的卫星速度
	{
		SatellitePosition satellitepos;
		if (v.first =="Point") 
		{
			string strValue = v.second.get_optional<string>("Location.X").get_value_or("0");
			satellitepos.dX = lexical_cast<double>(strValue); 

			strValue = v.second.get_optional<string>("Location.Y").get_value_or("0");
			satellitepos.dY = lexical_cast<double>(strValue);

			strValue = v.second.get_optional<string>("Location.Z").get_value_or("0");
			satellitepos.dZ = lexical_cast<double>(strValue);

			strValue = v.second.get_optional<string>("Velocity.X").get_value_or("0");
			satellitepos.dVx = lexical_cast<double>(strValue); 

			strValue = v.second.get_optional<string>("Velocity.Y").get_value_or("0");
			satellitepos.dVy = lexical_cast<double>(strValue);

			strValue = v.second.get_optional<string>("Velocity.Z").get_value_or("0");
			satellitepos.dVz = lexical_cast<double>(strValue);

			strValue = v.second.get_optional<string>("TIME").get_value_or("0");
			replace_all(strValue, "T", " ");
			satellitepos.ptm = time_from_string(strValue);

			spotDimParam.vPos.push_back(satellitepos);
		}
	}

	BOOST_FOREACH(ptree::value_type &v,	pt.get_child("Dimap_Document.Data_Strip.Satellite_Attitudes.Corrected_Attitudes.Corrected_Attitude")) //获取传感器姿态角
	{
		SatelliteAngle satelliteAngle;
		if (v.first =="Angles") 
		{
			string strValue = v.second.get_optional<string>("YAW").get_value_or("0");
			satelliteAngle.dYAW= lexical_cast<double>(strValue); 

			strValue = v.second.get_optional<string>("PITCH").get_value_or("0");
			satelliteAngle.dPITCH = lexical_cast<double>(strValue);

			strValue = v.second.get_optional<string>("ROLL").get_value_or("0");
			satelliteAngle.dROLL = lexical_cast<double>(strValue);

			strValue = v.second.get_optional<string>("TIME").get_value_or("0");
			replace_all(strValue, "T", " ");
			satelliteAngle.ptm = time_from_string(strValue);

			spotDimParam.vAngle.push_back(satelliteAngle);
		}
	}

	BOOST_FOREACH(ptree::value_type &v,	pt.get_child("Dimap_Document.Data_Strip.Satellite_Attitudes.Raw_Attitudes.Aocs_Attitude.Angular_Speeds_List")) //获取传感器姿态角速度
	{
		SatelliteAngle satelliteAngle;
		if (v.first =="Angular_Speeds") 
		{
			string strValue = v.second.get_optional<string>("YAW").get_value_or("0");
			satelliteAngle.dYAW= lexical_cast<double>(strValue); 

			strValue = v.second.get_optional<string>("PITCH").get_value_or("0");
			satelliteAngle.dPITCH = lexical_cast<double>(strValue);

			strValue = v.second.get_optional<string>("ROLL").get_value_or("0");
			satelliteAngle.dROLL = lexical_cast<double>(strValue);

			strValue = v.second.get_optional<string>("TIME").get_value_or("0");
			replace_all(strValue, "T", " ");
			satelliteAngle.ptm = time_from_string(strValue);

			spotDimParam.vAngleSpeed.push_back(satelliteAngle);
		}
	}

	return true;
}
ptime InterpolationT(SensorCenter sCenter,int iLine,double dLsp)
{
	return sCenter.ptm + seconds(dLsp *(iLine -sCenter.row));
}
void FindTimeAngle(SpotDimParam * DimPram ,SatelliteAngle &ang1,SatelliteAngle &ang2,ptime pt)
{
	if (!DimPram)
		return;

	vector<SatelliteAngle> vAngle = DimPram->vAngle;
	for (size_t i=0;i< vAngle.size();i++)
	{
		if (vAngle[i].ptm <pt)
			continue;
		else
		{
			ang1 = vAngle[i-1];
			ang2 = vAngle[i];
			return;
		}
	}

}
void  InterpolationSensorAngle(SatelliteAngle ang1,SatelliteAngle ang2,SatelliteAngle & outAng)
{
	time_duration  pti = outAng.ptm - ang1.ptm;
	time_duration  pt  = ang2.ptm-ang1.ptm;

	double df = pti.ticks() * 1.0/pt.ticks();

	double dTmp = (ang2.dPITCH - ang1.dPITCH);
	outAng.dPITCH = ang1.dPITCH +  dTmp *df;

	dTmp = (ang2.dROLL - ang1.dROLL);
	outAng.dROLL = ang1.dROLL +  dTmp *df;

	dTmp = (ang2.dYAW - ang1.dYAW);
	outAng.dYAW = ang1.dYAW +  dTmp *df;
}

void  InterpolationSensorPosition(SpotDimParam * DimPram,SatellitePosition  &SatellitePos)
{
	if (!DimPram)
		return ;

	vector<SatellitePosition> vPos = DimPram->vPos;
	int iCount = vPos.size();

	SatellitePos.dX  = 0;
	SatellitePos.dY  = 0;
	SatellitePos.dZ  = 0;
	SatellitePos.dVx = 0;
	SatellitePos.dVy = 0;
	SatellitePos.dVz = 0;

	for (int j=0;j<iCount;j++)
	{
		double dTConvNum =1.0;
		double dTConvDen =1.0;

		for (int i=0;i<iCount;i++)
		{
			if (i==j)
				continue;

			time_duration  tdNum = SatellitePos.ptm - vPos[i].ptm;
			dTConvNum *= tdNum.ticks(); //求分子的卷积

			time_duration  tdDen = vPos[j].ptm - vPos[i].ptm;
			dTConvDen  *= tdDen.ticks();  //求分母的卷积
		}


		SatellitePos.dX  += vPos[j].dX  * dTConvNum/dTConvDen;
		SatellitePos.dY  += vPos[j].dY  * dTConvNum/dTConvDen;
		SatellitePos.dZ  += vPos[j].dZ  * dTConvNum/dTConvDen;
		SatellitePos.dVx += vPos[j].dVx * dTConvNum/dTConvDen;
		SatellitePos.dVy += vPos[j].dVy * dTConvNum/dTConvDen;
		SatellitePos.dVz += vPos[j].dVz * dTConvNum/dTConvDen;
	}
}

void  InterpolationSensorAngle(SpotDimParam * DimPram,SatelliteAngle & outAng)
{
	SatelliteAngle ang1,ang2;
	FindTimeAngle(DimPram,ang1,ang2,outAng.ptm);
	InterpolationSensorAngle(ang1,ang2,outAng);
}

void  InterpolationAngleSpeed(SpotDimParam * DimPram,SatelliteAngleSpeed  &AngSpeed)
{
	if (!DimPram)
		return ;

	vector<SatelliteAngleSpeed> vAngSpeed = DimPram->vAngleSpeed;
	int iCount = vAngSpeed.size();

	AngSpeed.dYAW   = 0;
	AngSpeed.dPITCH = 0;
	AngSpeed.dROLL  = 0;

	for (int j=0;j<iCount;j++)
	{
		double dTConvNum =1.0;
		double dTConvDen =1.0;

		for (int i=0;i<iCount;i++)
		{
			if (i==j)
				continue;

			time_duration  tdNum = AngSpeed.ptm - vAngSpeed[i].ptm;
			dTConvNum *= tdNum.ticks()*1.0/1000000; //求分子的卷积

			time_duration  tdDen = vAngSpeed[j].ptm - vAngSpeed[i].ptm;
			dTConvDen  *= tdDen.ticks()*1.0/1000000;  //求分母的卷积
		}


		AngSpeed.dYAW    += vAngSpeed[j].dYAW    * dTConvNum/dTConvDen;
		AngSpeed.dPITCH  += vAngSpeed[j].dPITCH  * dTConvNum/dTConvDen;
		AngSpeed.dROLL   += vAngSpeed[j].dROLL   * dTConvNum/dTConvDen;
	}	
}
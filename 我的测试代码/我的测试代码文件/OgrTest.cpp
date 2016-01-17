#include <boost/progress.hpp>	//boost计时函数
#include "ogrsf_frmts.h"
#include "cpl_conv.h"
#include <vector>
using namespace std;



int Intersect(const char* pszInShp, const char* pszCutShp, const char* pszOutShp)
{
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");
	OGRRegisterAll();

	//打开数据
	OGRDataSource *poInDS = OGRSFDriverRegistrar::Open(pszInShp, FALSE );
	if( poInDS == NULL )
	{
		printf( "Open failed.\n" );
		return 1;
	}

	OGRDataSource *poCutDS = OGRSFDriverRegistrar::Open(pszCutShp, FALSE );
	if( poCutDS == NULL )
	{
		printf( "Open failed.\n" );
		OGRDataSource::DestroyDataSource( poInDS );
		return 1;
	}
	
	//创建数据
	const char *pszDriverName = "ESRI Shapefile";
	OGRSFDriver *poDriver;
	poDriver = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName(pszDriverName );
	if( poDriver == NULL )
	{
		printf( "%s driver not available.\n", pszDriverName );
		OGRDataSource::DestroyDataSource( poInDS );
		OGRDataSource::DestroyDataSource( poCutDS );
		return 2;
	}

	OGRDataSource *poOutDS;
	poOutDS = poDriver->CreateDataSource(pszOutShp, NULL );
	if( poOutDS == NULL )
	{
		printf( "Creation of output file failed.\n" );
		OGRDataSource::DestroyDataSource( poInDS );
		OGRDataSource::DestroyDataSource( poCutDS );
		return 2;
	}

	OGRLayer *poInLayer = poInDS->GetLayer( 0 );
	if( poInLayer == NULL )
	{
		printf( "Layer creation failed.\n" );
		OGRDataSource::DestroyDataSource( poInDS );
		OGRDataSource::DestroyDataSource( poCutDS );
		OGRDataSource::DestroyDataSource( poOutDS );
		return 3;
	}

	OGRLayer *poCutLayer = poCutDS->GetLayer( 0 );
	if( poCutLayer == NULL )
	{
		printf( "Layer creation failed.\n" );
		OGRDataSource::DestroyDataSource( poInDS );
		OGRDataSource::DestroyDataSource( poCutDS );
		OGRDataSource::DestroyDataSource( poOutDS );
		return 3;
	}

	OGRLayer *poOutLayer = poOutDS->CreateLayer( "polygon", NULL, wkbPolygon, NULL );
	if( poOutLayer == NULL )
	{
		printf( "Layer creation failed.\n" );
		OGRDataSource::DestroyDataSource( poInDS );
		OGRDataSource::DestroyDataSource( poCutDS );
		OGRDataSource::DestroyDataSource( poOutDS );
		return 3;
	}

	OGRFeature *poInFeature, *poCutFeature;

	poInLayer->ResetReading();
	poOutLayer->ResetReading();
	printf( "In Feature Count = %d\nCut Feature Count = %d\n", poInLayer->GetFeatureCount(), poCutLayer->GetFeatureCount() );

	int index = 0;
	while( (poInFeature = poInLayer->GetNextFeature()) != NULL )
	{
		printf( "In Feature ID = %d\n", index );
		index ++;

		OGRGeometry *poInGeometry = poInFeature->GetGeometryRef();
		if (poInGeometry == NULL || poInGeometry->IsEmpty())
		{
			OGRFeature::DestroyFeature( poInFeature );
			continue;
		}

		poCutLayer->ResetReading();
		poCutLayer->SetSpatialFilter(poInGeometry);

		while( (poCutFeature = poCutLayer->GetNextFeature()) != NULL )
		{
			OGRGeometry *poCutGeometry = poCutFeature->GetGeometryRef();
			OGRFeature *poOutFeature = NULL;
			poOutFeature = poCutFeature->Clone();
			//if (poCutGeometry == NULL || poCutGeometry->IsEmpty())
			//{
			//	OGRFeature::DestroyFeature( poCutFeature );
			//	continue;
			//}

			//if (poInGeometry->Contains(poCutGeometry))
			//{
			//	OGRFeature *poOutFeature = OGRFeature::CreateFeature( poOutLayer->GetLayerDefn() );
			//	poOutFeature->SetGeometry(poCutGeometry); 
			//}
			//else
			//{
			//	OGRGeometry* poOutGeometry = poInGeometry->Intersection(poCutGeometry);
			//	if (poOutGeometry != NULL && !poOutGeometry->IsEmpty())
			//	{
			//		OGRFeature *poOutFeature = OGRFeature::CreateFeature( poOutLayer->GetLayerDefn() );
			//		poOutFeature->SetGeometry(poOutGeometry); 
			//	}
			//}

			if(poOutFeature != NULL)
			{
				poOutLayer->CreateFeature( poOutFeature );
				OGRFeature::DestroyFeature( poOutFeature );
			}

			OGRFeature::DestroyFeature( poCutFeature );
		}

		OGRFeature::DestroyFeature( poInFeature );
	}

	OGRDataSource::DestroyDataSource( poInDS );
	OGRDataSource::DestroyDataSource( poCutDS );
	OGRDataSource::DestroyDataSource( poOutDS );

	return 0;
}

inline double GetArea(OGRGeometry* pGeometry)
{
	double dArea = 0.0;
	OGRwkbGeometryType type = pGeometry->getGeometryType();
	if (type == wkbPolygon)
	{
		OGRPolygon *pPoly = (OGRPolygon*)pGeometry;
		dArea = pPoly->get_Area();
	}
	else if (type == wkbMultiPolygon)
	{
		OGRMultiPolygon *pMultiPoly = (OGRMultiPolygon*)pGeometry;
		dArea = pMultiPoly->get_Area();
	}
	else if (type == wkbGeometryCollection)
	{
		OGRGeometryCollection *pGeometryCollection = (OGRGeometryCollection*)pGeometry;
		dArea = pGeometryCollection->get_Area();
	}

	return dArea;
}

double IntersectArea(const char* pszInShp, const char* pszCutShp, boost::progress_timer *pTime)
{
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");
	OGRRegisterAll();

	//打开数据
	OGRDataSource *poInDS = OGRSFDriverRegistrar::Open(pszInShp, FALSE );
	if( poInDS == NULL )
	{
		printf( "Open failed.\n" );
		return 1;
	}

	OGRDataSource *poCutDS = OGRSFDriverRegistrar::Open(pszCutShp, FALSE );
	if( poCutDS == NULL )
	{
		printf( "Open failed.\n" );
		OGRDataSource::DestroyDataSource( poInDS );
		return 1;
	}

	OGRLayer *poInLayer = poInDS->GetLayer( 0 );
	if( poInLayer == NULL )
	{
		printf( "Layer creation failed.\n" );
		OGRDataSource::DestroyDataSource( poInDS );
		OGRDataSource::DestroyDataSource( poCutDS );
		return 3;
	}

	OGRLayer *poCutLayer = poCutDS->GetLayer( 0 );
	if( poCutLayer == NULL )
	{
		printf( "Layer creation failed.\n" );
		OGRDataSource::DestroyDataSource( poInDS );
		OGRDataSource::DestroyDataSource( poCutDS );
		return 3;
	}

	double dArea = 0.0;
	OGRFeature *poInFeature, *poCutFeature;

	poInLayer->ResetReading();
	printf( "In Feature Count = %d\nCut Feature Count = %d\n", poInLayer->GetFeatureCount(), poCutLayer->GetFeatureCount() );

	printf( "time_start = %f\n", pTime->elapsed() );
	int index = 0;
	double dTempArea = 0.0;
	while( (poInFeature = poInLayer->GetNextFeature()) != NULL )
	{
		printf( "In Feature ID = %d\n", index );
		index ++;

		OGRGeometry *poInGeometry = poInFeature->GetGeometryRef();
		if (poInGeometry == NULL || poInGeometry->IsEmpty())
		{
			OGRFeature::DestroyFeature( poInFeature );
			continue;
		}

		printf( "time_Start = %f\n", pTime->elapsed() );
		poCutLayer->ResetReading();
		poCutLayer->SetSpatialFilter(poInGeometry);
		printf("过滤后个数:%d\n", poCutLayer->GetFeatureCount());

		while( (poCutFeature = poCutLayer->GetNextFeature()) != NULL )
		{
			OGRGeometry *poCutGeometry = poCutFeature->GetGeometryRef();
			if (poCutGeometry == NULL || poCutGeometry->IsEmpty())
			{
				OGRFeature::DestroyFeature( poCutFeature );
				continue;
			}

			dTempArea = 0.0;
			if (poInGeometry->Contains(poCutGeometry))
			{
				dTempArea = GetArea(poCutGeometry);
			}
			else
			{
				OGRGeometry* poOutGeometry = poInGeometry->Intersection(poCutGeometry);
				if (poOutGeometry != NULL && !poOutGeometry->IsEmpty())
					dTempArea = GetArea(poOutGeometry);
			}

			dArea += dTempArea;
			OGRFeature::DestroyFeature( poCutFeature );
		}

		printf( "time_End = %f\n", pTime->elapsed() );
		printf( "Cut Feature Area = %f\n", dArea );
		OGRFeature::DestroyFeature( poInFeature );
	}

	OGRDataSource::DestroyDataSource( poInDS );
	OGRDataSource::DestroyDataSource( poCutDS );

	printf( "All Area = %f\n", dArea );
	return dArea;
}

double IntersectArea2(const char* pszInShp, const char* pszCutShp)
{
	boost::progress_timer *pTime = new boost::progress_timer;
	printf( "开始处理 = %f\n", pTime->elapsed() );

	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");
	OGRRegisterAll();

	//打开数据
	OGRDataSource *poInDS = OGRSFDriverRegistrar::Open(pszInShp, FALSE );
	if( poInDS == NULL )
	{
		printf( "Open failed.\n" );
		return 1;
	}

	OGRDataSource *poCutDS = OGRSFDriverRegistrar::Open(pszCutShp, FALSE );
	if( poCutDS == NULL )
	{
		printf( "Open failed.\n" );
		OGRDataSource::DestroyDataSource( poInDS );
		return 1;
	}

	OGRLayer *poInLayer = poInDS->GetLayer( 0 );
	if( poInLayer == NULL )
	{
		printf( "Layer creation failed.\n" );
		OGRDataSource::DestroyDataSource( poInDS );
		OGRDataSource::DestroyDataSource( poCutDS );
		return 3;
	}

	OGRLayer *poCutLayer = poCutDS->GetLayer( 0 );
	if( poCutLayer == NULL )
	{
		printf( "Layer creation failed.\n" );
		OGRDataSource::DestroyDataSource( poInDS );
		OGRDataSource::DestroyDataSource( poCutDS );
		return 3;
	}

	double dArea = 0.0;
	double dTempArea = 0.0;

	poInLayer->ResetReading();
	poCutLayer->ResetReading();

	int iInCount = poInLayer->GetFeatureCount();

	printf( "In Feature Count = %d\n", iInCount);

	//将原始数据中的Geometry复制到数组中
	OGRGeometry **ppoInGeometry = new OGRGeometry *[iInCount];
	memset(ppoInGeometry, NULL, sizeof(OGRGeometry*)*iInCount);

	for (int i=0; i<iInCount; i++)
	{
		OGRFeature *pFeature = poInLayer->GetFeature(i);
		OGRGeometry *poGeometry = pFeature->GetGeometryRef();
		ppoInGeometry[i] = poGeometry->clone();
		OGRFeature::DestroyFeature( pFeature );
	}

	printf( "数据读取完毕 = %f\n", pTime->elapsed() );

	for(int i=0; i<iInCount; i++)
	{
		pTime->restart();
		OGRGeometry *poInGeometry = ppoInGeometry[i];
		if (poInGeometry == NULL || poInGeometry->IsEmpty())
			continue;

		//设置过滤条件
		poCutLayer->ResetReading();
		poCutLayer->SetSpatialFilter(poInGeometry);
		int iCutCount = poCutLayer->GetFeatureCount();
		printf( "Cut Feature Count = %d  过滤耗时 = %f\n", iCutCount, pTime->elapsed());
		pTime->restart();

		OGRGeometry **ppoCutGeometry = new OGRGeometry *[iCutCount];
		memset(ppoCutGeometry, NULL, sizeof(OGRGeometry*)*iCutCount);
		
		//读取过滤后的Geometry
		for (int j=0; j<iCutCount; j++)
		{
			OGRFeature *pFeature = poCutLayer->GetFeature(j);
			OGRGeometry *poGeometry = pFeature->GetGeometryRef();
			ppoCutGeometry[j] = poGeometry->clone();
			OGRFeature::DestroyFeature( pFeature );
		}

		for (int j=0; j<iCutCount; j++)
		{
			dTempArea = 0.0;

			OGRGeometry *poCutGeometry = ppoCutGeometry[j];
			if (poCutGeometry == NULL || poCutGeometry->IsEmpty())
			{
				OGRGeometryFactory::destroyGeometry(poCutGeometry);
				continue;
			}

			if (poInGeometry->Contains(poCutGeometry))
			{
				dTempArea = GetArea(poCutGeometry);
			}
			else
			{
				OGRGeometry* poOutGeometry = poInGeometry->Intersection(poCutGeometry);
				if (poOutGeometry != NULL && !poOutGeometry->IsEmpty())
					dTempArea = GetArea(poOutGeometry);

				OGRGeometryFactory::destroyGeometry(poOutGeometry);
			}

			OGRGeometryFactory::destroyGeometry(poCutGeometry);
			dArea += dTempArea;
		}

		delete []ppoCutGeometry;

		printf( "第%d个处理完毕，耗时 = %f\n", i+1, pTime->elapsed() );
	}

	//释放内存
	for (int i=0; i<iInCount; i++)
		OGRGeometryFactory::destroyGeometry(ppoInGeometry[i]);

	delete []ppoInGeometry;

	OGRDataSource::DestroyDataSource( poInDS );
	OGRDataSource::DestroyDataSource( poCutDS );

	printf( "All Area = %f\n", dArea );
	delete pTime;
	return dArea;
}

double IntersectArea3(const char* pszInShp, const char* pszCutShp)
{
	boost::progress_timer *pTime = new boost::progress_timer;
	printf( "开始处理 = %f\n", pTime->elapsed() );

	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");
	OGRRegisterAll();

	//打开数据
	OGRDataSource *poInDS = OGRSFDriverRegistrar::Open(pszInShp, FALSE );
	if( poInDS == NULL )
	{
		printf( "Open failed.\n" );
		return 1;
	}

	OGRDataSource *poCutDS = OGRSFDriverRegistrar::Open(pszCutShp, FALSE );
	if( poCutDS == NULL )
	{
		printf( "Open failed.\n" );
		OGRDataSource::DestroyDataSource( poInDS );
		return 1;
	}

	OGRLayer *poInLayer = poInDS->GetLayer( 0 );
	if( poInLayer == NULL )
	{
		printf( "Layer creation failed.\n" );
		OGRDataSource::DestroyDataSource( poInDS );
		OGRDataSource::DestroyDataSource( poCutDS );
		return 3;
	}

	OGRLayer *poCutLayer = poCutDS->GetLayer( 0 );
	if( poCutLayer == NULL )
	{
		printf( "Layer creation failed.\n" );
		OGRDataSource::DestroyDataSource( poInDS );
		OGRDataSource::DestroyDataSource( poCutDS );
		return 3;
	}

	double dArea = 0.0;
	double dTempArea = 0.0;

	poInLayer->ResetReading();
	poCutLayer->ResetReading();

	int iInCount = poInLayer->GetFeatureCount();
	printf( "In Feature Count = %d\n", iInCount);

	//将原始数据中的Geometry复制到数组中
	OGRGeometry **ppoInGeometry = new OGRGeometry *[iInCount];
	memset(ppoInGeometry, NULL, sizeof(OGRGeometry*)*iInCount);

	for (int i=0; i<iInCount; i++)
	{
		OGRFeature *pFeature = poInLayer->GetFeature(i);
		OGRGeometry *poGeometry = pFeature->GetGeometryRef();
		ppoInGeometry[i] = poGeometry->clone();
		OGRFeature::DestroyFeature( pFeature );
	}

	int iCutCount = poCutLayer->GetFeatureCount();
	printf( "Cut Feature Count = %d\n", iCutCount);

	//将原始数据中的Geometry复制到数组中
	vector<OGRGeometry *> ppoCutGeometry(iCutCount, NULL);
	for (int i=0; i<iCutCount; i++)
	{
		OGRFeature *pFeature = poCutLayer->GetFeature(i);
		OGRGeometry *poGeometry = pFeature->GetGeometryRef();
		ppoCutGeometry[i] = poGeometry->clone();
		OGRFeature::DestroyFeature( pFeature );
	}
	printf( "数据读取完毕 = %f\n", pTime->elapsed() );

	for(int i=0; i<iInCount; i++)
	{
		pTime->restart();
		OGRGeometry *poInGeometry = ppoInGeometry[i];
		if (poInGeometry == NULL || poInGeometry->IsEmpty())
			continue;

		vector<int> vEraseID;
		vector<OGRGeometry *>::const_iterator iter = ppoCutGeometry.begin();
		int j=0;
		for (iter = ppoCutGeometry.begin(); iter!=ppoCutGeometry.end(); iter++, j++)
		{
			dTempArea = 0.0;

			OGRGeometry *poCutGeometry = *iter;
			if (poCutGeometry == NULL || poCutGeometry->IsEmpty())
			{
				vEraseID.push_back(j);
				continue;
			}

			if (poInGeometry->Touches(poCutGeometry))
				continue;

			if (!poInGeometry->Intersects(poCutGeometry))
				continue;

			if (poInGeometry->Contains(poCutGeometry))
			{
				vEraseID.push_back(j);
				dTempArea = GetArea(poCutGeometry);
			}
			else
			{
				OGRGeometry* poOutGeometry = poInGeometry->Intersection(poCutGeometry);
				if (poOutGeometry != NULL && !poOutGeometry->IsEmpty())
					dTempArea = GetArea(poOutGeometry);
			}

			dArea += dTempArea;

			if(j%100 == 0)
				printf( ".");
		}

		printf( "done\n");

		if (!vEraseID.empty())
		{
			for (int j=vEraseID.size()-1; j>=0; j--)
			{
				iter = ppoCutGeometry.begin()+vEraseID[j];
				OGRGeometryFactory::destroyGeometry(*iter);
				ppoCutGeometry.erase(iter);
			}
			printf( "移除 %d ,面积 = %f\n", vEraseID.size(), dArea );
		}
		printf( "第%d个处理完毕,耗时 = %f\n", i+1, pTime->elapsed() );
	}

	//释放内存
	for (int i=0; i<iInCount; i++)
		OGRGeometryFactory::destroyGeometry(ppoInGeometry[i]);

	delete []ppoInGeometry;

	for (size_t j=ppoCutGeometry.size()-1; j>=0; j--)
	{
		OGRGeometryFactory::destroyGeometry(ppoCutGeometry[j]);
	}

	ppoCutGeometry.clear();

	OGRDataSource::DestroyDataSource( poInDS );
	OGRDataSource::DestroyDataSource( poCutDS );

	printf( "All Area = %f\n", dArea );
	delete pTime;
	return dArea;
}

double IntersectArea4(const char* pszInShp, const char* pszCutShp)
{
	boost::progress_timer *pTime = new boost::progress_timer;
	printf( "开始处理 = %f\n", pTime->elapsed() );

	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");
	OGRRegisterAll();

	//打开数据
	OGRDataSource *poInDS = OGRSFDriverRegistrar::Open(pszInShp, FALSE );
	if( poInDS == NULL )
	{
		printf( "Open failed.\n" );
		return 1;
	}

	OGRDataSource *poCutDS = OGRSFDriverRegistrar::Open(pszCutShp, FALSE );
	if( poCutDS == NULL )
	{
		printf( "Open failed.\n" );
		OGRDataSource::DestroyDataSource( poInDS );
		return 1;
	}

	OGRLayer *poInLayer = poInDS->GetLayer( 0 );
	if( poInLayer == NULL )
	{
		printf( "Layer creation failed.\n" );
		OGRDataSource::DestroyDataSource( poInDS );
		OGRDataSource::DestroyDataSource( poCutDS );
		return 3;
	}

	OGRLayer *poCutLayer = poCutDS->GetLayer( 0 );
	if( poCutLayer == NULL )
	{
		printf( "Layer creation failed.\n" );
		OGRDataSource::DestroyDataSource( poInDS );
		OGRDataSource::DestroyDataSource( poCutDS );
		return 3;
	}

	double dArea = 0.0;
	double dTempArea = 0.0;

	poInLayer->ResetReading();
	poCutLayer->ResetReading();

	int iInCount = poInLayer->GetFeatureCount();
	printf( "In Feature Count = %d\n", iInCount);

	//将原始数据中的Geometry复制到数组中
	OGRGeometry **ppoInGeometry = new OGRGeometry *[iInCount];
	memset(ppoInGeometry, NULL, sizeof(OGRGeometry*)*iInCount);

	for (int i=0; i<iInCount; i++)
	{
		OGRFeature *pFeature = poInLayer->GetFeature(i);
		OGRGeometry *poGeometry = pFeature->GetGeometryRef();
		ppoInGeometry[i] = poGeometry->clone();
		OGRFeature::DestroyFeature( pFeature );
	}

	int iCutCount = poCutLayer->GetFeatureCount();
	printf( "Cut Feature Count = %d\n", iCutCount);

	//将原始数据中的Geometry复制到数组中
	vector<OGRGeometry *> ppoCutGeometry(iCutCount, NULL);
	for (int i=0; i<iCutCount; i++)
	{
		OGRFeature *pFeature = poCutLayer->GetFeature(i);
		OGRGeometry *poGeometry = pFeature->GetGeometryRef();
		ppoCutGeometry[i] = poGeometry->clone();
		OGRFeature::DestroyFeature( pFeature );
	}
	printf( "数据读取完毕 = %f\n", pTime->elapsed() );

	for(int i=0; i<iInCount; i++)
	{
		pTime->restart();
		OGRGeometry *poInGeometry = ppoInGeometry[i];
		if (poInGeometry == NULL || poInGeometry->IsEmpty())
			continue;

		vector<int> vEraseID;
		vector<OGRGeometry *>::const_iterator iter = ppoCutGeometry.begin();
		int j=0;
		for (iter = ppoCutGeometry.begin(); iter!=ppoCutGeometry.end(); iter++, j++)
		{
			dTempArea = 0.0;

			OGRGeometry *poCutGeometry = *iter;
			if (poCutGeometry == NULL || poCutGeometry->IsEmpty())
			{
				vEraseID.push_back(j);
				continue;
			}

			if (poInGeometry->Touches(poCutGeometry))
				continue;

			if (!poInGeometry->Intersects(poCutGeometry))
				continue;

			if (poInGeometry->Contains(poCutGeometry))
			{
				vEraseID.push_back(j);
				dTempArea = GetArea(poCutGeometry);
			}
			else
			{
				OGRGeometry* poOutGeometry = poInGeometry->Intersection(poCutGeometry);
				if (poOutGeometry != NULL && !poOutGeometry->IsEmpty())
					dTempArea = GetArea(poOutGeometry);
			}

			dArea += dTempArea;

			if(j%100 == 0)
				printf( ".");
		}

		printf( "done\n");

		if (!vEraseID.empty())
		{
			for (int j=vEraseID.size()-1; j>=0; j--)
			{
				iter = ppoCutGeometry.begin()+vEraseID[j];
				OGRGeometryFactory::destroyGeometry(*iter);
				ppoCutGeometry.erase(iter);
			}
			printf( "移除 %d ,面积 = %f\n", vEraseID.size(), dArea );
		}
		printf( "第%d个处理完毕,耗时 = %f\n", i+1, pTime->elapsed() );
	}

	//释放内存
	for (int i=0; i<iInCount; i++)
		OGRGeometryFactory::destroyGeometry(ppoInGeometry[i]);

	delete []ppoInGeometry;

	for (size_t j=ppoCutGeometry.size()-1; j>=0; j--)
	{
		OGRGeometryFactory::destroyGeometry(ppoCutGeometry[j]);
	}

	ppoCutGeometry.clear();

	OGRDataSource::DestroyDataSource( poInDS );
	OGRDataSource::DestroyDataSource( poCutDS );

	printf( "All Area = %f\n", dArea );
	delete pTime;
	return dArea;
}

int main11()
{
	boost::progress_timer *pTime = new boost::progress_timer();  // 开始计时
	const char* pszInFile = "C:\\Documents and Settings\\Liml\\Desktop\\居住.shp";
	const char* pszCutFile = "C:\\Documents and Settings\\Liml\\Desktop\\所有绿地（即绿色信息）.shp";
	const char* pszOutFile = "C:\\Documents and Settings\\Liml\\Desktop\\Test1.shp";

	//int iRev = Intersect(pszInFile, pszCutFile, pszOutFile);
	double dArea = IntersectArea3(pszInFile, pszCutFile);
	printf( "Area = %f\n", dArea );

	delete pTime;
	::system("pause");
	return 0;
}
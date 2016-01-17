/***************************************************************************
*
* Time: 2013-05-21
* Project: 遥感平台
* Purpose: 实践九号数据预处理算法
* Author:  李民录
* Copyright (c) 2013, liminlu0314@gmail.com
* Describe:提供用于实践九号多光谱数据生成快视图，导出接口
*
****************************************************************************/
#include "SJ9DataProcessorAlg.h"
#include "ImageBatCommon.h"
#include "DataPreparation.h"

#include "gdal_priv.h"

#include "boost/format.hpp"
#include "boost/progress.hpp"	//boost计时函数
#include "boost/filesystem.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/xpressive/xpressive_dynamic.hpp"

#ifdef _WIN32
#include <windows.h>
#include <shellapi.h>
#include "D:\RsSrcDir\Release\FindPro.h"
#include "stdlib.h"
#endif

using namespace boost;
using namespace boost::filesystem;
using namespace boost::xpressive;

typedef recursive_directory_iterator rdir_iterator;	//递归目录迭代器
typedef directory_iterator dir_iterator;			//当前目录迭代器

#define TRaceTime(MSG,T,dLastTime) printf(MSG);\
	printf("，累计耗时%fs,单步耗时%fs\n",T.elapsed(),T.elapsed() - dLastTime);\
	dLastTime = T.elapsed();

extern void WriteInSJ9XmlFile(const char* ExplainingFileIn,const char* XmlFilePath);//外部函数声明

//创建文件夹，如果父目录不存在，自动创建
void RMakeDir(const char *pszPath)
{
	stack<string> st;
	string strTmp = pszPath;
	while(1)
	{
		if (chdir(strTmp.c_str())) //如果当前目录不存在
		{
			st.push(strTmp);
			strTmp = strTmp.substr(0,strTmp.find_last_of('\\')); //获取当前目录的父目录
		}
		else
			break;
	}

	while(!st.empty())
	{
		string str =st.top();
		mkdir(str.c_str());
		st.pop();
	}
}

static string ConstructDirectory(const char *p1, const char *p2, const char *p3, const char *p4,
								 const char *p5 = "", const char *pszExt = ".raw")
{
	format fmtPath("%s_%s_%s_%s%s%s");	//文件路径格式
	fmtPath % p1 % p2 % p3 % p4 % p5 % pszExt;
	return fmtPath.str();
}

static string ConstructFolderName(const char *pszInPutFileName)
{
	string strFileName = path(pszInPutFileName).stem();
	int ipos = strFileName.find("2");
	if (ipos!=string::npos)
		strFileName =strFileName.substr(ipos,strFileName.length());
	return strFileName;
}

struct CampareFileSize
{
	bool operator()(const path & leftFile,const path & rightFile) const
	{
		return (file_size(leftFile) > file_size(rightFile));
	}
};

CSJ9L0DataProduce::CSJ9L0DataProduce(const char *pszInPutFolder,const char *pszOutPutFolder,BOOL bBeijingStation)
{
	m_strInputFolder = pszInPutFolder;
	m_strParentOutputFolder = pszOutPutFolder;
	m_bBeijingStation = bBeijingStation;
	m_pBatProgress = NULL;
	m_pAllProcess  = NULL;
	m_pSubProcess  = NULL;

	string strTmp = SJ9_GetSystemParamters("SJ9A_TEMPDIR", CPLGenerateTempFilename("")); //临时数据目录
	printf("%s\n", strTmp.c_str());
	CPLSetConfigOption("SJ9A_TEMPDIR", strTmp.c_str());
}

CSJ9L0DataProduce::~CSJ9L0DataProduce()
{
	if(0)
	{
		remove( m_strTempFolder.c_str());
	}
}

bool CSJ9L0DataProduce::ScanInputFolder()
{
	static sregex_compiler rc;	//正则表达式工厂
	path dir = GetInputFolder();
	dir_iterator end;		//当前迭代器的末尾

	bool bMatchedCCD =false;
	bool bMatchedHSI =false;

	if (m_bBeijingStation)
	{
		path dirCCD,dirHSI;

		string strCCD = ".*SJ9A_SEPAMODE_CCD_(1|2)\\d{3}(0|1)\\d[0-3]\\d\\d{6}_\\d{9}$";
		string strHSI = ".*SJ9A_SEPAMODE_HSI_(1|2)\\d{3}(0|1)\\d[0-3]\\d\\d{6}_\\d{9}$";
		rc[strCCD] = rc.compile(strCCD, icase);		//创建一个正则表达式，icase表示大小写无关
		rc[strHSI] = rc.compile(strHSI, icase);		//创建一个正则表达式，icase表示大小写无关
		for(dir_iterator pos(dir); pos != end; ++pos)
		{
			if(is_directory(*pos) && regex_match(pos->path().string(), rc[strCCD]))
			{
				bMatchedCCD = true;
				dirCCD = *pos;
			}

			if(is_directory(*pos) && regex_match(pos->path().string(), rc[strHSI]))
			{
				bMatchedHSI = true;
				dirHSI = *pos;
			}
		}
		if (!bMatchedCCD || !bMatchedHSI)
			return false;

		char SZPATH[1024];
		for (int i=0;i<4;i++) //判断全色数据是否齐全
		{
			sprintf(SZPATH,"%s_%d.dat",dirCCD.filename().c_str(),i);
			path p = dirCCD /SZPATH;
			m_vstrCCDFile.push_back(p.string());
			if (!(!is_directory(p) &&exists(p)))
				return false;
		}
		for (int i=4;i<6;i++) //判断多光谱数据是否齐全
		{
			sprintf(SZPATH,"%s_%d.dat",dirHSI.filename().c_str(),i);
			path p = dirHSI /SZPATH;
			m_vstrHSIFile.push_back(p.string());
			if (!(!is_directory(p) &&exists(p)))
				return false;
		}
	}
	else
	{
		string strCCD = ".*SJ9A-PAN-(1|2)\\d{3}(0|1)\\d[0-3]\\d-\\d{6}-\\d{10}.dat$";
		string strHSI = ".*SJ9A-MUX-(1|2)\\d{3}(0|1)\\d[0-3]\\d-\\d{6}-\\d{10}.dat$";
		rc[strCCD] = rc.compile(strCCD, icase);		//创建一个正则表达式，icase表示大小写无关
		rc[strHSI] = rc.compile(strHSI, icase);		//创建一个正则表达式，icase表示大小写无关

		for(dir_iterator pos(dir); pos != end; ++pos)
		{
			if(!is_directory(*pos) && regex_match(pos->path().string(), rc[strCCD]))
			{
				m_vstrCCDFile.push_back(pos->path().string());
				bMatchedCCD = true;
			}
			if(!is_directory(*pos) && regex_match(pos->path().string(), rc[strHSI]))
			{
				m_vstrHSIFile.push_back(pos->path().string());
				bMatchedHSI = true;
			}
		}
		if (!bMatchedCCD || !bMatchedHSI)
			return false;
	}

	// 判断说明.txt文件是否存在
	path p = dir / "说明.txt";
	if (!(!is_directory(p) &&exists(p)))
		return false;

	return true;
}

void CSJ9L0DataProduce::MakeOutDir()
{
	m_strFinalOutputFolder = m_strParentOutputFolder + string("\\") + ConstructFolderName(m_vstrCCDFile[0].c_str());
	const char *pszOutFolderFinal = m_strFinalOutputFolder.c_str();
	RMakeDir(pszOutFolderFinal);
}

void CSJ9L0DataProduce::MakeTmpDir()
{
	string strTempFileName = CPLGenerateTempFilename("");
	const char* pszDir = CPLGetDirname(strTempFileName.c_str()); //获取父目录
	string strTemp 	=  CPLGetConfigOption("SJ9A_TEMPDIR",pszDir);
	m_strTempFolder = strTemp + "\\BeijingStation_" + ConstructFolderName(m_vstrCCDFile[0].c_str());;
}

int CSJ9L0DataProduce::DoDataExtractBeijing()
{
	string pInPutFile;
#ifdef _WIN32
	char ExeFile[260];
	CPLGetExecPath(ExeFile,260);
	string strExePath = CPLGetPath(ExeFile);
	for (int i=0;i<3;i++)
	{
		if (i<=3)
			pInPutFile = m_vstrCCDFile[i];
		else
			pInPutFile = m_vstrHSIFile[i-4];

		string strArgument = "-src ";
		strArgument += pInPutFile;
		strArgument += " -dst ";
		strArgument += m_strTempFolder;

		//strArgument = strExePath + string("\\SJ9DataExtract.exe ") + strArgument;
		//::system(strArgument.c_str());
		ShellExecute(NULL,NULL,"SJ9DataExtract.exe",strArgument.c_str(),strExePath.c_str(),1);
	}
	while(1)
	{
		if (FindProcByName("SJ9DataExtract.exe")==0)
			break;
		_sleep(5000);
	}
	for (int i=3;i<6;i++)
	{
		if (i<=3)
			pInPutFile = m_vstrCCDFile[i];
		else
			pInPutFile = m_vstrHSIFile[i-4];

		string strArgument = "-src ";
		strArgument += pInPutFile;
		strArgument += " -dst ";
		strArgument += m_strTempFolder;

		//strArgument = strExePath + string("\\SJ9DataExtract.exe ") + strArgument;
		//::system(strArgument.c_str());
		ShellExecute(NULL,NULL,"SJ9DataExtract.exe",strArgument.c_str(),strExePath.c_str(),1);
	}
	while(1)
	{
		if (FindProcByName("SJ9DataExtract.exe")==0)
			break;
		_sleep(5000);
	}
	return RE_SUCCESS;
#endif
	
	int  iRev = RE_FAILED;
	for (int i=0;i<6;i++)
	{
		if (i<=3)
			pInPutFile = m_vstrCCDFile[i];
		else
			pInPutFile = m_vstrHSIFile[i-4];

		iRev = SJ9_DataExtract(pInPutFile.c_str(), m_strTempFolder.c_str(),m_pSubProcess);
		if (m_pAllProcess)
			m_pAllProcess->StepIt();

		if (iRev != RE_SUCCESS)
		{
			printf("%s解码出错!\n",pInPutFile.c_str());
			return iRev;
		}
	}
	return RE_SUCCESS;
}

int CSJ9L0DataProduce::DoPanMergeBeijing(string & strOutPutMergedFile)
{
	char ** InPutList = NULL;
	int  iRev = FindMergeInPutFileList(&InPutList,true);

	int iRowStart[8], iRowCount;
	string strSatMetaFileNamePart = m_strFinalOutputFolder +  ConstructDirectory("\\SJ9A","PAN","20130530023033","002912","","");

	string txtFileIn = m_strInputFolder + "/说明.txt";
	string CreateXmlFilePath = strSatMetaFileNamePart +"_meta.xml";

	WriteInSJ9XmlFile(txtFileIn.c_str(),CreateXmlFilePath.c_str());

	SJ9_CalcRowStart(InPutList, strSatMetaFileNamePart.c_str(), iRowStart, iRowCount, TRUE, m_pSubProcess); //计算拼接时读取数据起始行

	if (m_pAllProcess)
		m_pAllProcess->StepIt();

	char ** InPutVrtList = NULL;
	double dGeotransform[6] = {0, 1, 0, 0, 0, -1};	//构造六参数
	int iOverSize = lexical_cast<int>(SJ9_GetSystemParamters("SJ9A_PAN_OFFSET", "40"));

	for (int i=0;i<8;i++)
	{
		if(i == 4)	//往前偏移80列
			dGeotransform[0] -= (2*iOverSize);

		string strVrtFile = string(InPutList[i]) + ".vrt";
		ConstructVrtFile(strVrtFile.c_str(), InPutList[i], 1536, iRowCount, 2, iRowStart[i]*3072, 3072, 2, TRUE, dGeotransform);
		InPutVrtList = CSLAddString(InPutVrtList, strVrtFile.c_str());

		dGeotransform[0] += 1536;
	}

	// 全色拼接
	strOutPutMergedFile = m_strTempFolder + ConstructDirectory("\\SJ9A","PAN","20130530023033","002912","_Pan_Merge",".vrt");
	iRev = SJ9_DataPanMerge(InPutVrtList, strOutPutMergedFile.c_str(), "VRT", m_pSubProcess);
	if (m_pAllProcess)
		m_pAllProcess->StepIt();

	if (iRev!=RE_SUCCESS)
	{
		printf("全色拼接出错!\n");
		return iRev;
	}

	return RE_SUCCESS;
}

int CSJ9L0DataProduce::FindMergeInPutFileList(char ***OutPutFileList,bool bIsPan)
{
	vector<path> vPathFiles;
	FindFiles(path(m_strTempFolder.c_str()), "*.raw", vPathFiles,false);
	std::sort(vPathFiles.begin(),vPathFiles.end(),CampareFileSize());
	int count = vPathFiles.size();
	if (count < 16)
		return RE_PARAMERROR;

	int sequence[8] = {count + 1,count + 1,count + 1,count + 1,count + 1,count + 1,count + 1,count + 1};

	if(bIsPan)
	{
		for (int i = 0; i < 8; i++) //取文件数据量较大的8个
		{
			string str = vPathFiles[i].string();
			str = str.substr(str.length()-8,4);
			if (str == "0_aa"||str == "1_aa")
				sequence[0] = i;
			else if (str == "0_bb"||str == "1_bb")
				sequence[1] = i;
			else if (str == "0_cc"||str == "1_cc")
				sequence[2] = i;
			else if (str == "0_dd"||str == "1_dd")
				sequence[3] = i;
			else if (str == "2_aa"||str == "3_aa")
				sequence[4] = i;
			else if (str == "2_bb"||str == "3_bb")
				sequence[5] = i;
			else if (str == "2_cc"||str == "3_cc")
				sequence[6] = i;
			else if (str == "2_dd"||str == "3_dd")
				sequence[7] = i;
		}
	}
	else
	{
		for (int i = 0; i < count; i++)
		{
			string str = vPathFiles[i].string();
			str = str.substr(str.length()-8,4);

			if (str == "4_aa")
				sequence[0] = i;
			else if (str == "4_bb")
				sequence[1] = i;
			else if (str == "4_cc")
				sequence[2] = i;
			else if (str == "4_dd")
				sequence[3] = i;
			else if (str == "5_aa")
				sequence[4] = i;
			else if (str == "5_bb")
				sequence[5] = i;
			else if (str == "5_cc")
				sequence[6] = i;
			else if (str == "5_dd")
				sequence[7] = i;
		}
	}

	for (int i=0;i<8;i++)
	{
		string strRawfile = vPathFiles[sequence[i]].string();
		*OutPutFileList = CSLAddString(*OutPutFileList,strRawfile.c_str());
	}

	return RE_SUCCESS;
}

int CSJ9L0DataProduce::DoPanRadiancyAndQuickPic(const string & strOutPanMergeFile)
{
	progress_timer ti;   // 开始计时
	// 全色辐射校正
	string strRadiancyFile = ConstructDirectory("\\SJ9A","PAN","20130530023033","002912","",".raw");
	strRadiancyFile = m_strFinalOutputFolder  + strRadiancyFile.c_str(); 
	int iRev = SJ9_DataPanRadiancy(strOutPanMergeFile.c_str(), strRadiancyFile.c_str(), FALSE, "ENVI", m_pSubProcess);
	printf("全色辐射校正耗时%f\n",ti.elapsed());
	if (m_pAllProcess)
		m_pAllProcess->StepIt();
	if (iRev!=RE_SUCCESS)
	{
		printf("全色辐射校正出错!\n");
		return iRev;
	}
	// 全色生产快视图
	progress_timer tp;   // 开始计时
	iRev = SJ9_CreateQP(strRadiancyFile.c_str(), m_pSubProcess);
	printf("全色生产快视图耗时%f\n",tp.elapsed());
	if (iRev!=RE_SUCCESS)
	{
		printf("全色数据生成快视图出错!\n");
		return iRev;
	}

	return RE_SUCCESS;
}

int CSJ9L0DataProduce::DoMsiMergeBeijing(char *** OutputMsiMergeList)
{
	char ** InPutList = NULL;
	int  iRev = FindMergeInPutFileList(&InPutList,false);

	int iRowStart[8],iRowCount;
	string strSatMetaFileNamePart = m_strFinalOutputFolder + ConstructDirectory("\\SJ9A","MUX","20130530023033","002912","","");

	string txtFileIn = m_strInputFolder + "/说明.txt";
	string CreateXmlFilePath = strSatMetaFileNamePart + "_meta.xml";

	WriteInSJ9XmlFile(txtFileIn.c_str(),CreateXmlFilePath.c_str());

	iRev = SJ9_CalcRowStart(InPutList, strSatMetaFileNamePart.c_str(), iRowStart, iRowCount, TRUE,m_pSubProcess); //取裁切的起始行和裁切行数并生成XML

	if (m_pAllProcess)
		m_pAllProcess->StepIt();

	if (iRev != RE_SUCCESS)
	{
		printf("计算抽头文件出错!\n");
		return iRev;
	}

	char ** InPutVrtList = NULL;
	double dGeotransform[6] = {0, 1, 0, 0, 0, -1};	//构造六参数
	int iOverSize = lexical_cast<int>(SJ9_GetSystemParamters("SJ9A_MUX_OFFSET", "10"));

	for (int i=0; i<8; i++)
	{
		if(i < 4)
			dGeotransform[0] = 0;
		else	//往前偏移20列
			dGeotransform[0] = 1536 - (2*iOverSize);

		string strRawfile = InPutList[i];
		string strVrtFile = strRawfile + ".vrt";
		ConstructVrtFile(strVrtFile.c_str(), strRawfile.c_str(), 1536, iRowCount, 2, iRowStart[i]*3072, 3072, 2, TRUE, dGeotransform);
		InPutVrtList = CSLAddString(InPutVrtList, strVrtFile.c_str());

		dGeotransform[0] += 1536;
	}

	// 多光谱拼接
	string file = m_strTempFolder + ConstructDirectory("\\SJ9A","MUX","20130530023033","002912","_B1_Merge",".vrt").c_str();
	*OutputMsiMergeList = CSLAddString(*OutputMsiMergeList, file.c_str());
	file = m_strTempFolder + ConstructDirectory("\\SJ9A","MUX","20130530023033","002912","_B2_Merge",".vrt").c_str();
	*OutputMsiMergeList = CSLAddString(*OutputMsiMergeList, file.c_str());
	file = m_strTempFolder + ConstructDirectory("\\SJ9A","MUX","20130530023033","002912","_B3_Merge",".vrt").c_str();
	*OutputMsiMergeList = CSLAddString(*OutputMsiMergeList, file.c_str());
	file = m_strTempFolder + ConstructDirectory("\\SJ9A","MUX","20130530023033","002912","_B4_Merge",".vrt").c_str();
	*OutputMsiMergeList = CSLAddString(*OutputMsiMergeList, file.c_str());

	iRev = SJ9_DataMsiMerge(InPutVrtList, *OutputMsiMergeList, "VRT", m_pSubProcess);
	if (iRev!=RE_SUCCESS)
	{
		printf("多光谱拼接出错!\n");
		return iRev;
	}
	if (m_pAllProcess)
		m_pAllProcess->StepIt();

	return RE_SUCCESS;
}

int CSJ9L0DataProduce::DoMsiRadiancyAndQuickPic(char **OutputMsiMergeOutList)
{
	int iRev = RE_FAILED;
	char ** OutputMsiFinalList = NULL;
	string file = m_strFinalOutputFolder + ConstructDirectory("\\SJ9A","MUX","20130530023033","002912","_B1",".raw").c_str();
	OutputMsiFinalList = CSLAddString(OutputMsiFinalList,file.c_str());
	file = m_strFinalOutputFolder + ConstructDirectory("\\SJ9A","MUX","20130530023033","002912","_B2",".raw").c_str();
	OutputMsiFinalList = CSLAddString(OutputMsiFinalList,file.c_str());
	file = m_strFinalOutputFolder + ConstructDirectory("\\SJ9A","MUX","20130530023033","002912","_B3",".raw").c_str();
	OutputMsiFinalList = CSLAddString(OutputMsiFinalList,file.c_str());
	file = m_strFinalOutputFolder + ConstructDirectory("\\SJ9A","MUX","20130530023033","002912","_B4",".raw").c_str();
	OutputMsiFinalList = CSLAddString(OutputMsiFinalList,file.c_str());

	double dLastTimeTR(0),dLastTimeTQ(0);
	progress_timer tR;   // 开始计时
	progress_timer tQ;   // 开始计时

	for(int i=0;i<4;i++)
	{
		// 多光谱辐射校正
		iRev = SJ9_DataMsiRadiancy(OutputMsiMergeOutList[i],OutputMsiFinalList[i],i,FALSE,"ENVI",m_pSubProcess);
		TRaceTime("多光谱辐射校正",tR,dLastTimeTR);
		if (iRev!=RE_SUCCESS)
		{
			printf("多光谱辐射校正出错!\n");
			return iRev;
		}
		// 多光谱生产快视图
		iRev = SJ9_CreateQP(OutputMsiFinalList[i],m_pSubProcess);
		TRaceTime("多光谱生产快视图",tQ,dLastTimeTQ);
		if (iRev!=RE_SUCCESS)
		{
			printf("多光谱生成快视图出错!\n");
			return iRev;
		}

		if (m_pAllProcess)
			m_pAllProcess->StepIt();
	}

	return RE_SUCCESS;
}

int CSJ9L0DataProduce::DoOtherStationProduce()
{
	int iRev = RE_FAILED;
	if (m_pAllProcess)
		m_pAllProcess->SetStepCount(13);

	double dLastTime = 0;
	progress_timer TimeDataExtract;   // 开始计时
	// 全色解码
	iRev = SJ9_DataExtract2(m_vstrCCDFile[0].c_str(), m_strTempFolder.c_str(), TRUE, m_pSubProcess);
	if (m_pAllProcess)
		m_pAllProcess->StepIt();

	TRaceTime("全色解码完成",TimeDataExtract,dLastTime);

	if (iRev!=RE_SUCCESS)
	{
		printf("%s解码出错!\n", m_vstrCCDFile[0].c_str());
		return iRev;
	}

	// 多光谱解码
	iRev = SJ9_DataExtract2(m_vstrHSIFile[0].c_str(), m_strTempFolder.c_str(), FALSE, m_pSubProcess);
	if (m_pAllProcess)
		m_pAllProcess->StepIt();

	TRaceTime("多光谱解码完成",TimeDataExtract,dLastTime);
	if (iRev!=RE_SUCCESS)
	{
		printf("%s解码出错!\n", m_vstrHSIFile[0].c_str());
		return iRev;
	}

	// 全色解析星历
	string strFileName = path(m_vstrCCDFile[0].c_str()).stem();
	string strPanExtract = m_strTempFolder + "\\" +strFileName + ".dat";
	string strSatMetaFileNamePart = m_strFinalOutputFolder +  ConstructDirectory("\\SJ9A","PAN","20130530023033","111111","","");
	iRev = SJ9_ParseAuxData(strPanExtract.c_str(), strSatMetaFileNamePart.c_str(), 0, 0, TRUE, m_pSubProcess);
	if (m_pAllProcess)
		m_pAllProcess->StepIt();

	TRaceTime("全色解析星历",TimeDataExtract,dLastTime);
	if (iRev!=RE_SUCCESS)
	{
		printf("%s解析星历出错!\n", strPanExtract.c_str());
		return iRev;
	}

	// 多光谱解析星历
	strFileName = path(m_vstrHSIFile[0].c_str()).stem();
	string strMuxExtractAux = m_strTempFolder + "\\" +strFileName + ".dat";
	strSatMetaFileNamePart = m_strFinalOutputFolder +  ConstructDirectory("\\SJ9A","MUX","20130530023033","111111","","");
	iRev = SJ9_ParseAuxData(strMuxExtractAux.c_str(), strSatMetaFileNamePart.c_str(), 0, 0, FALSE, m_pSubProcess);
	if (m_pAllProcess)
		m_pAllProcess->StepIt();

	TRaceTime("多光谱解析星历",TimeDataExtract,dLastTime);
	if (iRev!=RE_SUCCESS)
	{
		printf("%s解析星历出错!\n",strMuxExtractAux.c_str());
		return iRev;
	}


	// 全色辐射校正
	strFileName = path(m_vstrCCDFile[0].c_str()).stem();
	strPanExtract = m_strTempFolder + "\\" +strFileName + ".raw";
	string strRadiancyFile = ConstructDirectory("\\SJ9A","PAN","20130530023033","111111","",".raw");
	strRadiancyFile = m_strFinalOutputFolder  + strRadiancyFile.c_str(); 

	string strPanVrt = m_strTempFolder + "\\" +strFileName + ".vrt";

	filesystem::path pt(strPanExtract.c_str());
	int lineCount = static_cast<int>(file_size(pt) / 24416);

	iRev = ConstructVrtFile(strPanVrt.c_str(), strPanExtract.c_str(), 12208, lineCount, 2, 0, 24416, 2, TRUE);
	if (iRev!=RE_SUCCESS)
	{
		printf("构造VRT文件失败!\n");
		return iRev;
	}

	iRev = SJ9_DataPanRadiancy(strPanVrt.c_str(), strRadiancyFile.c_str(), FALSE, "ENVI", m_pSubProcess);
	if (m_pAllProcess)
		m_pAllProcess->StepIt();

	TRaceTime("全色辐射校正",TimeDataExtract,dLastTime);
	if (iRev!=RE_SUCCESS)
	{
		printf("全色辐射校正出错!\n");
		return iRev;
	}

	// 全色生成快视图
	iRev = SJ9_CreateQP(strRadiancyFile.c_str(), m_pSubProcess);
	TRaceTime("全色生成快视图",TimeDataExtract,dLastTime);


	// 生成辐射校正输入文件路径
	char ** OutputMsiExtractOutList = NULL;
	strFileName = path(m_vstrHSIFile[0].c_str()).stem();
	string strMuxExtract = m_strTempFolder + "\\" +strFileName;
	string  strBandFile  = strMuxExtract + "_B1.raw";
	OutputMsiExtractOutList = CSLAddString(OutputMsiExtractOutList,strBandFile.c_str());
	strBandFile  = strMuxExtract + "_B2.raw";
	OutputMsiExtractOutList = CSLAddString(OutputMsiExtractOutList,strBandFile.c_str());
	strBandFile  = strMuxExtract + "_B3.raw";
	OutputMsiExtractOutList = CSLAddString(OutputMsiExtractOutList,strBandFile.c_str());
	strBandFile  = strMuxExtract + "_B4.raw";
	OutputMsiExtractOutList = CSLAddString(OutputMsiExtractOutList,strBandFile.c_str());

	// 生成辐射校正输出文件路径
	char ** OutputMsiFinalList = NULL;
	string file = m_strFinalOutputFolder + ConstructDirectory("\\SJ9A","MUX","20130530023033","111111","_B1",".raw").c_str();
	OutputMsiFinalList = CSLAddString(OutputMsiFinalList,file.c_str());
	file = m_strFinalOutputFolder + ConstructDirectory("\\SJ9A","MUX","20130530023033","111111","_B2",".raw").c_str();
	OutputMsiFinalList = CSLAddString(OutputMsiFinalList,file.c_str());
	file = m_strFinalOutputFolder + ConstructDirectory("\\SJ9A","MUX","20130530023033","111111","_B3",".raw").c_str();
	OutputMsiFinalList = CSLAddString(OutputMsiFinalList,file.c_str());
	file = m_strFinalOutputFolder + ConstructDirectory("\\SJ9A","MUX","20130530023033","111111","_B4",".raw").c_str();
	OutputMsiFinalList = CSLAddString(OutputMsiFinalList,file.c_str());

	char strNameNum[100];
	for(int i=0;i<4;i++)
	{
		// 多光谱辐射校正
		sprintf(strNameNum,"_B%d.vrt",i+1);
		strFileName = path(m_vstrHSIFile[0].c_str()).stem();
		string strMuxVrt = m_strTempFolder + "\\" +strFileName + string(strNameNum);

		filesystem::path pt(OutputMsiExtractOutList[i]);
		int lineCount = static_cast<int>(file_size(pt) / 6104);

		iRev = ConstructVrtFile(strMuxVrt.c_str(), OutputMsiExtractOutList[i], 3052, lineCount, 2, 0, 6104, 2, TRUE);
		if (iRev!=RE_SUCCESS)
		{
			printf("构造VRT文件失败!\n");
			return iRev;
		}

		iRev = SJ9_DataMsiRadiancy(strMuxVrt.c_str(), OutputMsiFinalList[i], i, FALSE, "ENVI", m_pSubProcess);
		TRaceTime("多光谱辐射校正",TimeDataExtract,dLastTime);
		if (iRev!=RE_SUCCESS)
		{
			printf("多光谱辐射校正出错!\n");
			return iRev;
		}

		if (m_pAllProcess)
			m_pAllProcess->StepIt();

		// 多光谱生成快视图
		iRev = SJ9_CreateQP(OutputMsiFinalList[i], m_pSubProcess);
		TRaceTime("多光谱生成快视图",TimeDataExtract,dLastTime);
		if (iRev!=RE_SUCCESS)
		{
			printf("多光谱生成快视图出错!\n");
			return iRev;
		}

		if (m_pAllProcess)
			m_pAllProcess->StepIt();
	}

	return RE_SUCCESS;
}

int CSJ9L0DataProduce::ExcuteProduce(CBatProcessBase *pBatProgress)
{
	m_pBatProgress = pBatProgress;
	int iRev = RE_FAILED;
	if (m_pBatProgress!=NULL)
	{
		m_pBatProgress->SetMessage("正在执行实践九0级数据生产...");
		m_pAllProcess = m_pBatProgress->m_pAllProcess;
		m_pSubProcess = m_pBatProgress->m_pSubProcess;
	}

	bool bIsFolderOK = ScanInputFolder();
	if (!bIsFolderOK)
		return RE_PARAMERROR;

	MakeOutDir();
	MakeTmpDir();

	if (!m_bBeijingStation && m_vstrCCDFile.size()==1 && m_vstrHSIFile.size()==1)
		return DoOtherStationProduce();

	if (m_vstrCCDFile.size() + m_vstrHSIFile.size() !=6)
		return RE_PARAMERROR;

	if (m_pAllProcess)
		m_pAllProcess->SetStepCount(15);

	double dLastTime = 0;
	progress_timer TimeDataExtract;   // 开始计时
	iRev = DoDataExtractBeijing();
	TRaceTime("解码完成",TimeDataExtract,dLastTime);
	if (iRev !=RE_SUCCESS)
		return iRev;

	string strOutPanMergeFile;
	iRev =  DoPanMergeBeijing(strOutPanMergeFile);
	TRaceTime("全色拼接完成",TimeDataExtract,dLastTime);
	if (iRev !=RE_SUCCESS)
		return iRev;

	iRev =  DoPanRadiancyAndQuickPic(strOutPanMergeFile);
	TRaceTime("全色辐射校正和生成快视图完成",TimeDataExtract,dLastTime);
	if (iRev !=RE_SUCCESS)
		return iRev;

	char ** OutputMsiFinalList = NULL;
	iRev = DoMsiMergeBeijing(&OutputMsiFinalList);
	TRaceTime("多光谱拼接完成",TimeDataExtract,dLastTime);
	if (iRev !=RE_SUCCESS)
		return iRev;

	iRev = DoMsiRadiancyAndQuickPic(OutputMsiFinalList);
	TRaceTime("多光谱辐射校正和生成快视图完成",TimeDataExtract,dLastTime);

	return iRev;
}
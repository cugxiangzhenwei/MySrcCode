#define ICE_STATIC_LIBS
#include <IceUtil/IceUtil.h>
#include <Ice/Ice.h>
#include <IceStorm/IceStorm.h>

#include <StormNewsDemo.h>
#include "IceService.h"

using namespace std;
using namespace Pera;

extern vector<string> g_vClientIdList;

void main()
{
	Ice::InitializationData initData;
	initData.properties = Ice::createProperties();
	initData.properties->setProperty("Ice.MessageSizeMax", "102400" );//默认是1024，单位KB
	initData.properties->setProperty("Ice.ThreadPool.Server.Size", "1");
	initData.properties->setProperty("Ice.ThreadPool.Server.SizeMax", "1000" );
	initData.properties->setProperty("Ice.ThreadPool.Server.SizeWarn", "1024");
	Ice::CommunicatorPtr communicatorPtr = Ice::initialize(initData);


	char szStormHost[100]={0};
	char szStromPort[10]={0};

	char szDir[MAX_PATH] = {0};
	GetModuleFileName(NULL,szDir,MAX_PATH);
	string strIniFile = szDir;
	strIniFile = strIniFile.substr(0,strIniFile.length()-3) + "ini";

	GetPrivateProfileString("NewsPub","StormHost","localhost",szStormHost,100,strIniFile.c_str());
	WritePrivateProfileString("NewsPub","StormHost",szStormHost,strIniFile.c_str());

	GetPrivateProfileString("NewsPub","StromPort","10000",szStromPort,100,strIniFile.c_str());
	WritePrivateProfileString("NewsPub","StromPort",szStromPort,strIniFile.c_str());

	char szStr[1000]={0};
	sprintf_s(szStr,"StormNewsDemo/TopicManager:tcp -h %s -p %s",szStormHost,szStromPort);

	// icestorm的地址"StormNewsDemo/TopicManager:tcp -h xiangzhenwei.peraportal.com -p 10000"
	IceStorm::TopicManagerPrx manager = NULL;
	try
	{
		 manager = IceStorm::TopicManagerPrx::checkedCast(communicatorPtr->stringToProxy(szStr));
	}
	catch (const Ice::Exception &e)
	{
		cerr << e.what();
		return;
	}
	
	CIceService ServiecObj;
	ServiecObj.StartService();

	IceStorm::TopicPrx topic;
	try
	{
		topic = manager->retrieve("news");
	}
	catch(const IceStorm::NoSuchTopic&)
	{
		try
		{
			topic = manager->create("news");
		}
		catch(const IceStorm::TopicExists&)
		{
			cerr << "NewsPub.exe" << ": temporary failure. try again." << endl;
			return;
		}
	}

	//
	// Get the topic's publisher object, and create a Clock proxy with
	// the mode specified as an argument of this application.
	//
	Ice::ObjectPrx publisher = topic->getPublisher();
	publisher = publisher->ice_oneway();

	NewsPrx news = NewsPrx::uncheckedCast(publisher);

	cout << "publishing tick events. Press ^C to terminate the application." << endl;

	int iCountIndex = 0;
	try
	{
		while(true)
		{
			iCountIndex++;
			if (iCountIndex >= (int)g_vClientIdList.size())
				iCountIndex = 0;

			if (g_vClientIdList.empty())
			{
				IceUtil::ThreadControl::sleep(IceUtil::Time::seconds(1));
				continue;
			}

			char szMsgContent[400]={0};
			sprintf_s(szMsgContent,"消息内容：%s,现在时间：%s",g_vClientIdList[iCountIndex].c_str(),IceUtil::Time::now().toDateTime().c_str());
			char szNewMsg[1000] = {0};
			sprintf_s(szNewMsg,"消息发送给%s,%s\n",g_vClientIdList[iCountIndex].c_str(),szMsgContent);
			printf(szNewMsg);
			news->NewMsg(szMsgContent);
			IceUtil::ThreadControl::sleep(IceUtil::Time::seconds(1));
		}
	}
	catch(const Ice::CommunicatorDestroyedException&)
	{
		// Ignore
	}

}
#define ICE_STATIC_LIBS
#include <IceUtil/IceUtil.h>
#include <Ice/Ice.h>
#include <IceStorm/IceStorm.h>

#include <StormNewsDemo.h>

using namespace std;
using namespace Pera;

string g_strClientId = "";

Ice::CommunicatorPtr InitCommunicator()
{
	Ice::InitializationData initData;
	initData.properties = Ice::createProperties();
	initData.properties->setProperty("Ice.MessageSizeMax", "1024");//默认是1024，单位KB
	Ice::CommunicatorPtr communicatorPtr = Ice::initialize(initData);
	return communicatorPtr;
}


class NewsI : public News
{
public:

	virtual void
		NewMsg(const string& msg, const Ice::Current&)
	{
		string strClientId = msg.substr(10,g_strClientId.length());
		if (strClientId!= g_strClientId)
			return;

		cout << msg << endl;
	}
};

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

	if(!manager)
	{
		cerr << "NewsSub.exe" << ": invalid proxy" << endl;
		return;
	}

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
			cerr << "NewsSub.exe" << ": temporary failure. try again." << endl;
			return;
		}
	}
	// 接收端监听消息的地址"tcp -h 0.0.0.0:udp -h 0.0.0.0"
	string strEndPoint = "tcp -h 0.0.0.0:udp -h 0.0.0.0";
	Ice::ObjectAdapterPtr adapter = communicatorPtr->createObjectAdapterWithEndpoints("News.Subscriber", strEndPoint );

	//
	// Add a servant for the Ice object. If --id is used the identity
	// comes from the command line, otherwise a UUID is used.
	//
	// id is not directly altered since it is used below to detect
	// whether subscribeAndGetPublisher can raise AlreadySubscribed.
	//
	Ice::Identity subId;
	subId.name = IceUtil::generateUUID();
	Ice::ObjectPrx subscriber = adapter->add(new NewsI, subId);
	g_strClientId = subId.name;

	Ice::CommunicatorPtr communicatorPtr2 = InitCommunicator();
	char szEndPoints[1000]={0};
	sprintf_s(szEndPoints,"Pera601DemoServerService:tcp -h %s -p %s -t 5000", szStormHost, "20131");
	try
	{
		PcIdToWsServerPrx m_pPrx = PcIdToWsServerPrx::checkedCast(communicatorPtr2->stringToProxy(szEndPoints));
		m_pPrx = m_pPrx->ice_twoway()->ice_timeout(20000)->ice_secure(false);
		m_pPrx->TellClientId(subId.name);	 	
	}
	catch(const Ice::Exception& ex)
	{
		printf("远程调用服务端失败,ICE异常：%s", ex.ice_name().c_str());
		return;
	}


	//
	// Activate the object adapter before subscribing.
	//
	adapter->activate();
	subscriber = subscriber->ice_oneway();
	 IceStorm::QoS qos;
	 qos["retryCount"] = 3;

	try
	{
		topic->subscribeAndGetPublisher(qos, subscriber);
	}
	catch(const IceStorm::AlreadySubscribed&)
	{
		// If we're manually setting the subscriber id ignore.
		cout << "reactivating persistent subscriber" << endl;
	}

	communicatorPtr->waitForShutdown();
	topic->unsubscribe(subscriber);
}
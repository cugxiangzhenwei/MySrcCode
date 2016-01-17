#include <stdio.h>
#include "MyClass.h"
#include "ice/Application.h"

class  MyApp:virtual public Ice::Application
{
	virtual int run(int ,char *[]);
};
int MyApp::run(int argc,char *argv[])
{
	Ice::CommunicatorPtr ic;  
	//建立通信器  
	ic=Ice::initialize(argc, argv);  
	//建立对象适配器，绑定端口  
	Ice::ObjectAdapterPtr adapter=ic->createObjectAdapterWithEndpoints(  
		"MyIceCoreAdapter","default -p 10000");  
	Ice::ObjectPtr object = new MyCore();  
	//加入Ice对象实例，指定标识符  
	adapter->add(object,ic->stringToIdentity("MyIceCore"));  
	//启动适配器  
	adapter->activate();  
	//挂起该进程
	cout<< "等待客户端响应..."<<endl;
	ic->waitForShutdown();  
	return 0;
}
int main(int argc,char *argv[])
{
	MyApp app;
	return app.main(argc,argv);
}
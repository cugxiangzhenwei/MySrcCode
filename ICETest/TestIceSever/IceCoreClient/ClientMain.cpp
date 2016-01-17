#include <Ice/Ice.h>  
#include "IceCore.h"  
using namespace std;  
using namespace CoreSpace;  
Ice::CommunicatorPtr ic;  
int main(int argc,char *argv[])
{
	//建立通信器  
	ic = Ice::initialize(argc, argv);  
	//获得Ice对象代理，SimplePrinter-对象标识符，default -p 10000-协议与端口  
	Ice::ObjectPrx base = ic->stringToProxy("MyIceCore:default -p 10000");  
	//向下转换  
	CoreBasePrx co = CoreBasePrx::checkedCast(base);  
	if (!co) throw "Invalid proxy";  
	//调用操作  
	co->PrintMsg("Hello World!");  
	int iSum = co->add(98,2);
	cout << iSum <<endl;
	int iSub = co->sub(87,7);
	cout<<iSub<<endl;
	ic->destroy(); // 清理资源

	return 0;
}

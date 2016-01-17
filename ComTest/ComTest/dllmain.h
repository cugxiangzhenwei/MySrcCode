// dllmain.h : 模块类的声明。

class CComTestModule : public CAtlDllModuleT< CComTestModule >
{
public :
	DECLARE_LIBID(LIBID_ComTestLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_COMTEST, "{44CC24A8-6929-4016-A4F2-EFFC7BA75DBE}")
};

extern class CComTestModule _AtlModule;

// dllmain.h : 模块类的声明。

class CFileProUnityModule : public ATL::CAtlDllModuleT< CFileProUnityModule >
{
public :
	DECLARE_LIBID(LIBID_FileProUnityLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_FILEPROUNITY, "{1CBFA084-1DAB-424C-9B83-BB0C7326CBB0}")
};

extern class CFileProUnityModule _AtlModule;

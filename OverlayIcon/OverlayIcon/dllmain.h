// dllmain.h : 模块类的声明。

class COverlayIconModule : public ATL::CAtlDllModuleT< COverlayIconModule >
{
public :
	DECLARE_LIBID(LIBID_OverlayIconLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_OVERLAYICON, "{9C45F6BD-C6CD-4C92-8C97-8A43EE2A41F7}")
};

extern class COverlayIconModule _AtlModule;

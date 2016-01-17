// ComBase.h : CComBase 的声明

#pragma once
#include "ComTest_i.h"
#include "resource.h"       // 主符号
#include <comsvcs.h>



// CComBase

class ATL_NO_VTABLE CComBase :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CComBase, &CLSID_ComBase>,
	public IDispatchImpl<IComBase, &IID_IComBase, &LIBID_ComTestLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IDispatchImpl<IComponentRegistrar, &__uuidof(IComponentRegistrar), &LIBID_ComTestLib, /* wMajor = */ 1, /* wMinor = */ 0>,
	public IElevatedRegistration
{
public:
	CComBase()
	{
	}

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_COMBASE)

	DECLARE_NOT_AGGREGATABLE(CComBase)

	BEGIN_COM_MAP(CComBase)
		COM_INTERFACE_ENTRY(IComBase)
		COM_INTERFACE_ENTRY2(IDispatch, IComponentRegistrar)
		COM_INTERFACE_ENTRY(IComponentRegistrar)
		COM_INTERFACE_ENTRY(IElevatedRegistration)
	END_COM_MAP()




	// IComBase
public:
	STDMETHOD(PrintMsg)(CHAR* msg);

	// IComponentRegistrar Methods
public:
	STDMETHOD(Attach)( BSTR bstrPath)
	{
		// 在此处添加函数实现。
		return E_NOTIMPL;
	}
	STDMETHOD(RegisterAll)()
	{
		// 在此处添加函数实现。
		return E_NOTIMPL;
	}
	STDMETHOD(UnregisterAll)()
	{
		// 在此处添加函数实现。
		return E_NOTIMPL;
	}
	STDMETHOD(GetComponents)(SAFEARRAY * * pbstrCLSIDs, SAFEARRAY * * pbstrDescriptions)
	{
		// 在此处添加函数实现。
		return E_NOTIMPL;
	}
	STDMETHOD(RegisterComponent)( BSTR bstrCLSID)
	{
		// 在此处添加函数实现。
		return E_NOTIMPL;
	}
	STDMETHOD(UnregisterComponent)( BSTR bstrCLSID)
	{
		// 在此处添加函数实现。
		return E_NOTIMPL;
	}
	STDMETHOD(Sum)(LONG iStart, LONG iEnd);

	// IElevatedRegistration Methods
public:
	STDMETHOD(WriteRegistrationData)(BSTR RegKey, BSTR Name, BSTR Company, BSTR ProductKey)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(DeleteRegistrationData)(BSTR RegKey)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(CanModifyRegistrationData)(BSTR RegKey)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(WriteActivationData)(BSTR RegKey, BSTR ActivationKey)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(DeleteActivationData)(BSTR RegKey)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(CanModifyActivationData)(BSTR RegKey)
	{
		return E_NOTIMPL;
	}
};

OBJECT_ENTRY_AUTO(__uuidof(ComBase), CComBase)

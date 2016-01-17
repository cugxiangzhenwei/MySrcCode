

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Sat Mar 15 22:56:23 2014
 */
/* Compiler settings for FileProUnity.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __FileProUnity_i_h__
#define __FileProUnity_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IFileProCom_FWD_DEFINED__
#define __IFileProCom_FWD_DEFINED__
typedef interface IFileProCom IFileProCom;
#endif 	/* __IFileProCom_FWD_DEFINED__ */


#ifndef ___IFileProComEvents_FWD_DEFINED__
#define ___IFileProComEvents_FWD_DEFINED__
typedef interface _IFileProComEvents _IFileProComEvents;
#endif 	/* ___IFileProComEvents_FWD_DEFINED__ */


#ifndef __FileProCom_FWD_DEFINED__
#define __FileProCom_FWD_DEFINED__

#ifdef __cplusplus
typedef class FileProCom FileProCom;
#else
typedef struct FileProCom FileProCom;
#endif /* __cplusplus */

#endif 	/* __FileProCom_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IFileProCom_INTERFACE_DEFINED__
#define __IFileProCom_INTERFACE_DEFINED__

/* interface IFileProCom */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IFileProCom;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("DF88EC29-172B-40AB-9269-82E37515EB68")
    IFileProCom : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE OpenFileDlg( 
            /* [in] */ BSTR strInJson,
            /* [retval][out] */ BSTR *strOutJson) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SelectDirDlg( 
            /* [in] */ BSTR strJsonIn,
            /* [retval][out] */ BSTR *strJsonOut) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetFileInfo( 
            /* [in] */ BSTR strJSonIn,
            /* [retval][out] */ BSTR *strOutJson) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DeleteLocalFile( 
            /* [in] */ BSTR strInJson,
            /* [retval][out] */ BSTR *strOutJson) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetFileMd5( 
            /* [in] */ BSTR strInJson,
            /* [retval][out] */ BSTR *strOutJson) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE OpenFile( 
            /* [in] */ BSTR strInJson,
            /* [retval][out] */ BSTR *strOutJson) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CopyFileCom( 
            /* [in] */ BSTR strInJson,
            /* [retval][out] */ BSTR *strOutJson) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetCopyProgress( 
            /* [in] */ BSTR strInJson,
            /* [retval][out] */ BSTR *strOutJson) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetJsCallbackFunc( 
            /* [in] */ VARIANT scriptCallback) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CancelCopy( 
            /* [in] */ BSTR strInJson) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetAllNewItemTypes( 
            /* [in] */ BSTR strInJson,
            /* [retval][out] */ BSTR *strOutJson) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateNewFile( 
            /* [in] */ BSTR strInJson,
            /* [retval][out] */ BSTR *strOutJson) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IFileProComVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IFileProCom * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IFileProCom * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IFileProCom * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IFileProCom * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IFileProCom * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IFileProCom * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IFileProCom * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *OpenFileDlg )( 
            IFileProCom * This,
            /* [in] */ BSTR strInJson,
            /* [retval][out] */ BSTR *strOutJson);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SelectDirDlg )( 
            IFileProCom * This,
            /* [in] */ BSTR strJsonIn,
            /* [retval][out] */ BSTR *strJsonOut);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetFileInfo )( 
            IFileProCom * This,
            /* [in] */ BSTR strJSonIn,
            /* [retval][out] */ BSTR *strOutJson);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DeleteLocalFile )( 
            IFileProCom * This,
            /* [in] */ BSTR strInJson,
            /* [retval][out] */ BSTR *strOutJson);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetFileMd5 )( 
            IFileProCom * This,
            /* [in] */ BSTR strInJson,
            /* [retval][out] */ BSTR *strOutJson);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *OpenFile )( 
            IFileProCom * This,
            /* [in] */ BSTR strInJson,
            /* [retval][out] */ BSTR *strOutJson);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CopyFileCom )( 
            IFileProCom * This,
            /* [in] */ BSTR strInJson,
            /* [retval][out] */ BSTR *strOutJson);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetCopyProgress )( 
            IFileProCom * This,
            /* [in] */ BSTR strInJson,
            /* [retval][out] */ BSTR *strOutJson);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetJsCallbackFunc )( 
            IFileProCom * This,
            /* [in] */ VARIANT scriptCallback);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CancelCopy )( 
            IFileProCom * This,
            /* [in] */ BSTR strInJson);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetAllNewItemTypes )( 
            IFileProCom * This,
            /* [in] */ BSTR strInJson,
            /* [retval][out] */ BSTR *strOutJson);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateNewFile )( 
            IFileProCom * This,
            /* [in] */ BSTR strInJson,
            /* [retval][out] */ BSTR *strOutJson);
        
        END_INTERFACE
    } IFileProComVtbl;

    interface IFileProCom
    {
        CONST_VTBL struct IFileProComVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IFileProCom_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IFileProCom_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IFileProCom_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IFileProCom_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IFileProCom_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IFileProCom_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IFileProCom_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IFileProCom_OpenFileDlg(This,strInJson,strOutJson)	\
    ( (This)->lpVtbl -> OpenFileDlg(This,strInJson,strOutJson) ) 

#define IFileProCom_SelectDirDlg(This,strJsonIn,strJsonOut)	\
    ( (This)->lpVtbl -> SelectDirDlg(This,strJsonIn,strJsonOut) ) 

#define IFileProCom_GetFileInfo(This,strJSonIn,strOutJson)	\
    ( (This)->lpVtbl -> GetFileInfo(This,strJSonIn,strOutJson) ) 

#define IFileProCom_DeleteLocalFile(This,strInJson,strOutJson)	\
    ( (This)->lpVtbl -> DeleteLocalFile(This,strInJson,strOutJson) ) 

#define IFileProCom_GetFileMd5(This,strInJson,strOutJson)	\
    ( (This)->lpVtbl -> GetFileMd5(This,strInJson,strOutJson) ) 

#define IFileProCom_OpenFile(This,strInJson,strOutJson)	\
    ( (This)->lpVtbl -> OpenFile(This,strInJson,strOutJson) ) 

#define IFileProCom_CopyFileCom(This,strInJson,strOutJson)	\
    ( (This)->lpVtbl -> CopyFileCom(This,strInJson,strOutJson) ) 

#define IFileProCom_GetCopyProgress(This,strInJson,strOutJson)	\
    ( (This)->lpVtbl -> GetCopyProgress(This,strInJson,strOutJson) ) 

#define IFileProCom_SetJsCallbackFunc(This,scriptCallback)	\
    ( (This)->lpVtbl -> SetJsCallbackFunc(This,scriptCallback) ) 

#define IFileProCom_CancelCopy(This,strInJson)	\
    ( (This)->lpVtbl -> CancelCopy(This,strInJson) ) 

#define IFileProCom_GetAllNewItemTypes(This,strInJson,strOutJson)	\
    ( (This)->lpVtbl -> GetAllNewItemTypes(This,strInJson,strOutJson) ) 

#define IFileProCom_CreateNewFile(This,strInJson,strOutJson)	\
    ( (This)->lpVtbl -> CreateNewFile(This,strInJson,strOutJson) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IFileProCom_INTERFACE_DEFINED__ */



#ifndef __FileProUnityLib_LIBRARY_DEFINED__
#define __FileProUnityLib_LIBRARY_DEFINED__

/* library FileProUnityLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_FileProUnityLib;

#ifndef ___IFileProComEvents_DISPINTERFACE_DEFINED__
#define ___IFileProComEvents_DISPINTERFACE_DEFINED__

/* dispinterface _IFileProComEvents */
/* [uuid] */ 


EXTERN_C const IID DIID__IFileProComEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("0D092B26-73F9-491E-9954-3FABF0BDB32F")
    _IFileProComEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _IFileProComEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _IFileProComEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _IFileProComEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _IFileProComEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _IFileProComEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _IFileProComEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _IFileProComEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _IFileProComEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _IFileProComEventsVtbl;

    interface _IFileProComEvents
    {
        CONST_VTBL struct _IFileProComEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IFileProComEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _IFileProComEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _IFileProComEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _IFileProComEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _IFileProComEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _IFileProComEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _IFileProComEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___IFileProComEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_FileProCom;

#ifdef __cplusplus

class DECLSPEC_UUID("47ED8616-159C-45BC-8EA3-E77FEBEEF28F")
FileProCom;
#endif
#endif /* __FileProUnityLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long *, unsigned long            , VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserMarshal(  unsigned long *, unsigned char *, VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserUnmarshal(unsigned long *, unsigned char *, VARIANT * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long *, VARIANT * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif



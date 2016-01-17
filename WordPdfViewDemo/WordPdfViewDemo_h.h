

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Sun Jun 01 23:23:05 2014
 */
/* Compiler settings for WordPdfViewDemo.idl:
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


#ifndef __WordPdfViewDemo_h_h__
#define __WordPdfViewDemo_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IWordPdfViewDemo_FWD_DEFINED__
#define __IWordPdfViewDemo_FWD_DEFINED__
typedef interface IWordPdfViewDemo IWordPdfViewDemo;
#endif 	/* __IWordPdfViewDemo_FWD_DEFINED__ */


#ifndef __CWordPdfViewDemoDoc_FWD_DEFINED__
#define __CWordPdfViewDemoDoc_FWD_DEFINED__

#ifdef __cplusplus
typedef class CWordPdfViewDemoDoc CWordPdfViewDemoDoc;
#else
typedef struct CWordPdfViewDemoDoc CWordPdfViewDemoDoc;
#endif /* __cplusplus */

#endif 	/* __CWordPdfViewDemoDoc_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __WordPdfViewDemo_LIBRARY_DEFINED__
#define __WordPdfViewDemo_LIBRARY_DEFINED__

/* library WordPdfViewDemo */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_WordPdfViewDemo;

#ifndef __IWordPdfViewDemo_DISPINTERFACE_DEFINED__
#define __IWordPdfViewDemo_DISPINTERFACE_DEFINED__

/* dispinterface IWordPdfViewDemo */
/* [uuid] */ 


EXTERN_C const IID DIID_IWordPdfViewDemo;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("EAC5604D-2E09-41AB-B632-DCD3D12194C1")
    IWordPdfViewDemo : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IWordPdfViewDemoVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWordPdfViewDemo * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWordPdfViewDemo * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWordPdfViewDemo * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IWordPdfViewDemo * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IWordPdfViewDemo * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IWordPdfViewDemo * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IWordPdfViewDemo * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IWordPdfViewDemoVtbl;

    interface IWordPdfViewDemo
    {
        CONST_VTBL struct IWordPdfViewDemoVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWordPdfViewDemo_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWordPdfViewDemo_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWordPdfViewDemo_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWordPdfViewDemo_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IWordPdfViewDemo_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IWordPdfViewDemo_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IWordPdfViewDemo_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IWordPdfViewDemo_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_CWordPdfViewDemoDoc;

#ifdef __cplusplus

class DECLSPEC_UUID("84D33554-6E4F-4A60-B3F2-4A409F361F8C")
CWordPdfViewDemoDoc;
#endif
#endif /* __WordPdfViewDemo_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif



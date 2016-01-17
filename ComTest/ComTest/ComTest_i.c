

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 7.00.0500 */
/* at Tue Jul 23 08:17:25 2013
 */
/* Compiler settings for .\ComTest.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_IComponentRegistrar,0xa817e7a2,0x43fa,0x11d0,0x9e,0x44,0x00,0xaa,0x00,0xb6,0x77,0x0a);


MIDL_DEFINE_GUID(IID, IID_IComBase,0x7CEC6352,0x7381,0x4733,0xB9,0x7A,0x86,0x4F,0xEC,0x47,0x7A,0x71);


MIDL_DEFINE_GUID(IID, LIBID_ComTestLib,0xF6EE483C,0x5653,0x4154,0x96,0xA8,0xDF,0xDE,0xD6,0x96,0x52,0xB9);


MIDL_DEFINE_GUID(CLSID, CLSID_CompReg,0x6D91A6BB,0x1D27,0x402B,0x9B,0x87,0x51,0x30,0x53,0x37,0x04,0xFC);


MIDL_DEFINE_GUID(CLSID, CLSID_ComBase,0x775DE05F,0x1EA2,0x4E29,0xA1,0x3B,0x0E,0x93,0xD6,0x73,0xC8,0xF8);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif




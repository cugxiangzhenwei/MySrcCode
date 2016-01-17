

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


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

MIDL_DEFINE_GUID(IID, IID_IFileProCom,0xDF88EC29,0x172B,0x40AB,0x92,0x69,0x82,0xE3,0x75,0x15,0xEB,0x68);


MIDL_DEFINE_GUID(IID, LIBID_FileProUnityLib,0x9B0A1516,0x481B,0x4857,0xB8,0xAA,0xA6,0x02,0x60,0xB8,0x16,0x8A);


MIDL_DEFINE_GUID(IID, DIID__IFileProComEvents,0x0D092B26,0x73F9,0x491E,0x99,0x54,0x3F,0xAB,0xF0,0xBD,0xB3,0x2F);


MIDL_DEFINE_GUID(CLSID, CLSID_FileProCom,0x47ED8616,0x159C,0x45BC,0x8E,0xA3,0xE7,0x7F,0xEB,0xEE,0xF2,0x8F);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif




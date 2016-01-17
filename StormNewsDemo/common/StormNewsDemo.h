// **********************************************************************
//
// Copyright (c) 2003-2013 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************
//
// Ice version 3.5.0
//
// <auto-generated>
//
// Generated from file `StormNewsDemo.ice'
//
// Warning: do not edit this file.
//
// </auto-generated>
//

#ifndef __StormNewsDemo_h__
#define __StormNewsDemo_h__

#include <Ice/ProxyF.h>
#include <Ice/ObjectF.h>
#include <Ice/Exception.h>
#include <Ice/LocalObject.h>
#include <Ice/StreamHelpers.h>
#include <Ice/Proxy.h>
#include <Ice/Object.h>
#include <Ice/Outgoing.h>
#include <Ice/OutgoingAsync.h>
#include <Ice/Incoming.h>
#include <Ice/Direct.h>
#include <IceUtil/ScopedArray.h>
#include <IceUtil/Optional.h>
#include <Ice/StreamF.h>
#include <Ice/UndefSysMacros.h>

#ifndef ICE_IGNORE_VERSION
#   if ICE_INT_VERSION / 100 != 305
#       error Ice version mismatch!
#   endif
#   if ICE_INT_VERSION % 100 > 50
#       error Beta header file detected
#   endif
#   if ICE_INT_VERSION % 100 < 0
#       error Ice patch level mismatch!
#   endif
#endif

namespace IceProxy
{

namespace Pera
{

class News;
void __read(::IceInternal::BasicStream*, ::IceInternal::ProxyHandle< ::IceProxy::Pera::News>&);
::IceProxy::Ice::Object* upCast(::IceProxy::Pera::News*);

class PcIdToWsServer;
void __read(::IceInternal::BasicStream*, ::IceInternal::ProxyHandle< ::IceProxy::Pera::PcIdToWsServer>&);
::IceProxy::Ice::Object* upCast(::IceProxy::Pera::PcIdToWsServer*);

}

}

namespace Pera
{

class News;
bool operator==(const News&, const News&);
bool operator<(const News&, const News&);
::Ice::Object* upCast(::Pera::News*);
typedef ::IceInternal::Handle< ::Pera::News> NewsPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::Pera::News> NewsPrx;
void __patch(NewsPtr&, const ::Ice::ObjectPtr&);

class PcIdToWsServer;
bool operator==(const PcIdToWsServer&, const PcIdToWsServer&);
bool operator<(const PcIdToWsServer&, const PcIdToWsServer&);
::Ice::Object* upCast(::Pera::PcIdToWsServer*);
typedef ::IceInternal::Handle< ::Pera::PcIdToWsServer> PcIdToWsServerPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::Pera::PcIdToWsServer> PcIdToWsServerPrx;
void __patch(PcIdToWsServerPtr&, const ::Ice::ObjectPtr&);

}

namespace Pera
{

class Callback_News_NewMsg_Base : virtual public ::IceInternal::CallbackBase { };
typedef ::IceUtil::Handle< Callback_News_NewMsg_Base> Callback_News_NewMsgPtr;

class Callback_PcIdToWsServer_TellClientId_Base : virtual public ::IceInternal::CallbackBase { };
typedef ::IceUtil::Handle< Callback_PcIdToWsServer_TellClientId_Base> Callback_PcIdToWsServer_TellClientIdPtr;

}

namespace IceProxy
{

namespace Pera
{

class News : virtual public ::IceProxy::Ice::Object
{
public:

    void NewMsg(const ::std::string& time)
    {
        NewMsg(time, 0);
    }
    void NewMsg(const ::std::string& time, const ::Ice::Context& __ctx)
    {
        NewMsg(time, &__ctx);
    }
#ifdef ICE_CPP11
    ::Ice::AsyncResultPtr
    begin_NewMsg(const ::std::string& time, const ::IceInternal::Function<void ()>& response, const ::IceInternal::Function<void (const ::Ice::Exception&)>& exception = ::IceInternal::Function<void (const ::Ice::Exception&)>(), const ::IceInternal::Function<void (bool)>& sent = ::IceInternal::Function<void (bool)>())
    {
        return begin_NewMsg(time, 0, new ::IceInternal::Cpp11FnOnewayCallbackNC(response, exception, sent));
    }
    ::Ice::AsyncResultPtr
    begin_NewMsg(const ::std::string& time, const ::IceInternal::Function<void (const ::Ice::AsyncResultPtr&)>& completed, const ::IceInternal::Function<void (const ::Ice::AsyncResultPtr&)>& sent = ::IceInternal::Function<void (const ::Ice::AsyncResultPtr&)>())
    {
        return begin_NewMsg(time, 0, ::Ice::newCallback(completed, sent), 0);
    }
    ::Ice::AsyncResultPtr
    begin_NewMsg(const ::std::string& time, const ::Ice::Context& ctx, const ::IceInternal::Function<void ()>& response, const ::IceInternal::Function<void (const ::Ice::Exception&)>& exception = ::IceInternal::Function<void (const ::Ice::Exception&)>(), const ::IceInternal::Function<void (bool)>& sent = ::IceInternal::Function<void (bool)>())
    {
        return begin_NewMsg(time, &ctx, new ::IceInternal::Cpp11FnOnewayCallbackNC(response, exception, sent), 0);
    }
    ::Ice::AsyncResultPtr
    begin_NewMsg(const ::std::string& time, const ::Ice::Context& ctx, const ::IceInternal::Function<void (const ::Ice::AsyncResultPtr&)>& completed, const ::IceInternal::Function<void (const ::Ice::AsyncResultPtr&)>& sent = ::IceInternal::Function<void (const ::Ice::AsyncResultPtr&)>())
    {
        return begin_NewMsg(time, &ctx, ::Ice::newCallback(completed, sent));
    }
#endif

    ::Ice::AsyncResultPtr begin_NewMsg(const ::std::string& time)
    {
        return begin_NewMsg(time, 0, ::IceInternal::__dummyCallback, 0);
    }

    ::Ice::AsyncResultPtr begin_NewMsg(const ::std::string& time, const ::Ice::Context& __ctx)
    {
        return begin_NewMsg(time, &__ctx, ::IceInternal::__dummyCallback, 0);
    }

    ::Ice::AsyncResultPtr begin_NewMsg(const ::std::string& time, const ::Ice::CallbackPtr& __del, const ::Ice::LocalObjectPtr& __cookie = 0)
    {
        return begin_NewMsg(time, 0, __del, __cookie);
    }

    ::Ice::AsyncResultPtr begin_NewMsg(const ::std::string& time, const ::Ice::Context& __ctx, const ::Ice::CallbackPtr& __del, const ::Ice::LocalObjectPtr& __cookie = 0)
    {
        return begin_NewMsg(time, &__ctx, __del, __cookie);
    }

    ::Ice::AsyncResultPtr begin_NewMsg(const ::std::string& time, const ::Pera::Callback_News_NewMsgPtr& __del, const ::Ice::LocalObjectPtr& __cookie = 0)
    {
        return begin_NewMsg(time, 0, __del, __cookie);
    }

    ::Ice::AsyncResultPtr begin_NewMsg(const ::std::string& time, const ::Ice::Context& __ctx, const ::Pera::Callback_News_NewMsgPtr& __del, const ::Ice::LocalObjectPtr& __cookie = 0)
    {
        return begin_NewMsg(time, &__ctx, __del, __cookie);
    }

    void end_NewMsg(const ::Ice::AsyncResultPtr&);
    
private:

    void NewMsg(const ::std::string&, const ::Ice::Context*);
    ::Ice::AsyncResultPtr begin_NewMsg(const ::std::string&, const ::Ice::Context*, const ::IceInternal::CallbackBasePtr&, const ::Ice::LocalObjectPtr& __cookie = 0);
    
public:
    
    ::IceInternal::ProxyHandle<News> ice_context(const ::Ice::Context& __context) const
    {
        return dynamic_cast<News*>(::IceProxy::Ice::Object::ice_context(__context).get());
    }
    
    ::IceInternal::ProxyHandle<News> ice_adapterId(const ::std::string& __id) const
    {
        return dynamic_cast<News*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    }
    
    ::IceInternal::ProxyHandle<News> ice_endpoints(const ::Ice::EndpointSeq& __endpoints) const
    {
        return dynamic_cast<News*>(::IceProxy::Ice::Object::ice_endpoints(__endpoints).get());
    }
    
    ::IceInternal::ProxyHandle<News> ice_locatorCacheTimeout(int __timeout) const
    {
        return dynamic_cast<News*>(::IceProxy::Ice::Object::ice_locatorCacheTimeout(__timeout).get());
    }
    
    ::IceInternal::ProxyHandle<News> ice_connectionCached(bool __cached) const
    {
        return dynamic_cast<News*>(::IceProxy::Ice::Object::ice_connectionCached(__cached).get());
    }
    
    ::IceInternal::ProxyHandle<News> ice_endpointSelection(::Ice::EndpointSelectionType __est) const
    {
        return dynamic_cast<News*>(::IceProxy::Ice::Object::ice_endpointSelection(__est).get());
    }
    
    ::IceInternal::ProxyHandle<News> ice_secure(bool __secure) const
    {
        return dynamic_cast<News*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    }
    
    ::IceInternal::ProxyHandle<News> ice_preferSecure(bool __preferSecure) const
    {
        return dynamic_cast<News*>(::IceProxy::Ice::Object::ice_preferSecure(__preferSecure).get());
    }
    
    ::IceInternal::ProxyHandle<News> ice_router(const ::Ice::RouterPrx& __router) const
    {
        return dynamic_cast<News*>(::IceProxy::Ice::Object::ice_router(__router).get());
    }
    
    ::IceInternal::ProxyHandle<News> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
        return dynamic_cast<News*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    }
    
    ::IceInternal::ProxyHandle<News> ice_collocationOptimized(bool __co) const
    {
        return dynamic_cast<News*>(::IceProxy::Ice::Object::ice_collocationOptimized(__co).get());
    }
    
    ::IceInternal::ProxyHandle<News> ice_twoway() const
    {
        return dynamic_cast<News*>(::IceProxy::Ice::Object::ice_twoway().get());
    }
    
    ::IceInternal::ProxyHandle<News> ice_oneway() const
    {
        return dynamic_cast<News*>(::IceProxy::Ice::Object::ice_oneway().get());
    }
    
    ::IceInternal::ProxyHandle<News> ice_batchOneway() const
    {
        return dynamic_cast<News*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    }
    
    ::IceInternal::ProxyHandle<News> ice_datagram() const
    {
        return dynamic_cast<News*>(::IceProxy::Ice::Object::ice_datagram().get());
    }
    
    ::IceInternal::ProxyHandle<News> ice_batchDatagram() const
    {
        return dynamic_cast<News*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    }
    
    ::IceInternal::ProxyHandle<News> ice_compress(bool __compress) const
    {
        return dynamic_cast<News*>(::IceProxy::Ice::Object::ice_compress(__compress).get());
    }
    
    ::IceInternal::ProxyHandle<News> ice_timeout(int __timeout) const
    {
        return dynamic_cast<News*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    }
    
    ::IceInternal::ProxyHandle<News> ice_connectionId(const ::std::string& __id) const
    {
        return dynamic_cast<News*>(::IceProxy::Ice::Object::ice_connectionId(__id).get());
    }
    
    ::IceInternal::ProxyHandle<News> ice_encodingVersion(const ::Ice::EncodingVersion& __v) const
    {
        return dynamic_cast<News*>(::IceProxy::Ice::Object::ice_encodingVersion(__v).get());
    }
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
    virtual ::IceProxy::Ice::Object* __newInstance() const;
};

class PcIdToWsServer : virtual public ::IceProxy::Ice::Object
{
public:

    void TellClientId(const ::std::string& strClientId)
    {
        TellClientId(strClientId, 0);
    }
    void TellClientId(const ::std::string& strClientId, const ::Ice::Context& __ctx)
    {
        TellClientId(strClientId, &__ctx);
    }
#ifdef ICE_CPP11
    ::Ice::AsyncResultPtr
    begin_TellClientId(const ::std::string& strClientId, const ::IceInternal::Function<void ()>& response, const ::IceInternal::Function<void (const ::Ice::Exception&)>& exception = ::IceInternal::Function<void (const ::Ice::Exception&)>(), const ::IceInternal::Function<void (bool)>& sent = ::IceInternal::Function<void (bool)>())
    {
        return begin_TellClientId(strClientId, 0, new ::IceInternal::Cpp11FnOnewayCallbackNC(response, exception, sent));
    }
    ::Ice::AsyncResultPtr
    begin_TellClientId(const ::std::string& strClientId, const ::IceInternal::Function<void (const ::Ice::AsyncResultPtr&)>& completed, const ::IceInternal::Function<void (const ::Ice::AsyncResultPtr&)>& sent = ::IceInternal::Function<void (const ::Ice::AsyncResultPtr&)>())
    {
        return begin_TellClientId(strClientId, 0, ::Ice::newCallback(completed, sent), 0);
    }
    ::Ice::AsyncResultPtr
    begin_TellClientId(const ::std::string& strClientId, const ::Ice::Context& ctx, const ::IceInternal::Function<void ()>& response, const ::IceInternal::Function<void (const ::Ice::Exception&)>& exception = ::IceInternal::Function<void (const ::Ice::Exception&)>(), const ::IceInternal::Function<void (bool)>& sent = ::IceInternal::Function<void (bool)>())
    {
        return begin_TellClientId(strClientId, &ctx, new ::IceInternal::Cpp11FnOnewayCallbackNC(response, exception, sent), 0);
    }
    ::Ice::AsyncResultPtr
    begin_TellClientId(const ::std::string& strClientId, const ::Ice::Context& ctx, const ::IceInternal::Function<void (const ::Ice::AsyncResultPtr&)>& completed, const ::IceInternal::Function<void (const ::Ice::AsyncResultPtr&)>& sent = ::IceInternal::Function<void (const ::Ice::AsyncResultPtr&)>())
    {
        return begin_TellClientId(strClientId, &ctx, ::Ice::newCallback(completed, sent));
    }
#endif

    ::Ice::AsyncResultPtr begin_TellClientId(const ::std::string& strClientId)
    {
        return begin_TellClientId(strClientId, 0, ::IceInternal::__dummyCallback, 0);
    }

    ::Ice::AsyncResultPtr begin_TellClientId(const ::std::string& strClientId, const ::Ice::Context& __ctx)
    {
        return begin_TellClientId(strClientId, &__ctx, ::IceInternal::__dummyCallback, 0);
    }

    ::Ice::AsyncResultPtr begin_TellClientId(const ::std::string& strClientId, const ::Ice::CallbackPtr& __del, const ::Ice::LocalObjectPtr& __cookie = 0)
    {
        return begin_TellClientId(strClientId, 0, __del, __cookie);
    }

    ::Ice::AsyncResultPtr begin_TellClientId(const ::std::string& strClientId, const ::Ice::Context& __ctx, const ::Ice::CallbackPtr& __del, const ::Ice::LocalObjectPtr& __cookie = 0)
    {
        return begin_TellClientId(strClientId, &__ctx, __del, __cookie);
    }

    ::Ice::AsyncResultPtr begin_TellClientId(const ::std::string& strClientId, const ::Pera::Callback_PcIdToWsServer_TellClientIdPtr& __del, const ::Ice::LocalObjectPtr& __cookie = 0)
    {
        return begin_TellClientId(strClientId, 0, __del, __cookie);
    }

    ::Ice::AsyncResultPtr begin_TellClientId(const ::std::string& strClientId, const ::Ice::Context& __ctx, const ::Pera::Callback_PcIdToWsServer_TellClientIdPtr& __del, const ::Ice::LocalObjectPtr& __cookie = 0)
    {
        return begin_TellClientId(strClientId, &__ctx, __del, __cookie);
    }

    void end_TellClientId(const ::Ice::AsyncResultPtr&);
    
private:

    void TellClientId(const ::std::string&, const ::Ice::Context*);
    ::Ice::AsyncResultPtr begin_TellClientId(const ::std::string&, const ::Ice::Context*, const ::IceInternal::CallbackBasePtr&, const ::Ice::LocalObjectPtr& __cookie = 0);
    
public:
    
    ::IceInternal::ProxyHandle<PcIdToWsServer> ice_context(const ::Ice::Context& __context) const
    {
        return dynamic_cast<PcIdToWsServer*>(::IceProxy::Ice::Object::ice_context(__context).get());
    }
    
    ::IceInternal::ProxyHandle<PcIdToWsServer> ice_adapterId(const ::std::string& __id) const
    {
        return dynamic_cast<PcIdToWsServer*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    }
    
    ::IceInternal::ProxyHandle<PcIdToWsServer> ice_endpoints(const ::Ice::EndpointSeq& __endpoints) const
    {
        return dynamic_cast<PcIdToWsServer*>(::IceProxy::Ice::Object::ice_endpoints(__endpoints).get());
    }
    
    ::IceInternal::ProxyHandle<PcIdToWsServer> ice_locatorCacheTimeout(int __timeout) const
    {
        return dynamic_cast<PcIdToWsServer*>(::IceProxy::Ice::Object::ice_locatorCacheTimeout(__timeout).get());
    }
    
    ::IceInternal::ProxyHandle<PcIdToWsServer> ice_connectionCached(bool __cached) const
    {
        return dynamic_cast<PcIdToWsServer*>(::IceProxy::Ice::Object::ice_connectionCached(__cached).get());
    }
    
    ::IceInternal::ProxyHandle<PcIdToWsServer> ice_endpointSelection(::Ice::EndpointSelectionType __est) const
    {
        return dynamic_cast<PcIdToWsServer*>(::IceProxy::Ice::Object::ice_endpointSelection(__est).get());
    }
    
    ::IceInternal::ProxyHandle<PcIdToWsServer> ice_secure(bool __secure) const
    {
        return dynamic_cast<PcIdToWsServer*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    }
    
    ::IceInternal::ProxyHandle<PcIdToWsServer> ice_preferSecure(bool __preferSecure) const
    {
        return dynamic_cast<PcIdToWsServer*>(::IceProxy::Ice::Object::ice_preferSecure(__preferSecure).get());
    }
    
    ::IceInternal::ProxyHandle<PcIdToWsServer> ice_router(const ::Ice::RouterPrx& __router) const
    {
        return dynamic_cast<PcIdToWsServer*>(::IceProxy::Ice::Object::ice_router(__router).get());
    }
    
    ::IceInternal::ProxyHandle<PcIdToWsServer> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
        return dynamic_cast<PcIdToWsServer*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    }
    
    ::IceInternal::ProxyHandle<PcIdToWsServer> ice_collocationOptimized(bool __co) const
    {
        return dynamic_cast<PcIdToWsServer*>(::IceProxy::Ice::Object::ice_collocationOptimized(__co).get());
    }
    
    ::IceInternal::ProxyHandle<PcIdToWsServer> ice_twoway() const
    {
        return dynamic_cast<PcIdToWsServer*>(::IceProxy::Ice::Object::ice_twoway().get());
    }
    
    ::IceInternal::ProxyHandle<PcIdToWsServer> ice_oneway() const
    {
        return dynamic_cast<PcIdToWsServer*>(::IceProxy::Ice::Object::ice_oneway().get());
    }
    
    ::IceInternal::ProxyHandle<PcIdToWsServer> ice_batchOneway() const
    {
        return dynamic_cast<PcIdToWsServer*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    }
    
    ::IceInternal::ProxyHandle<PcIdToWsServer> ice_datagram() const
    {
        return dynamic_cast<PcIdToWsServer*>(::IceProxy::Ice::Object::ice_datagram().get());
    }
    
    ::IceInternal::ProxyHandle<PcIdToWsServer> ice_batchDatagram() const
    {
        return dynamic_cast<PcIdToWsServer*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    }
    
    ::IceInternal::ProxyHandle<PcIdToWsServer> ice_compress(bool __compress) const
    {
        return dynamic_cast<PcIdToWsServer*>(::IceProxy::Ice::Object::ice_compress(__compress).get());
    }
    
    ::IceInternal::ProxyHandle<PcIdToWsServer> ice_timeout(int __timeout) const
    {
        return dynamic_cast<PcIdToWsServer*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    }
    
    ::IceInternal::ProxyHandle<PcIdToWsServer> ice_connectionId(const ::std::string& __id) const
    {
        return dynamic_cast<PcIdToWsServer*>(::IceProxy::Ice::Object::ice_connectionId(__id).get());
    }
    
    ::IceInternal::ProxyHandle<PcIdToWsServer> ice_encodingVersion(const ::Ice::EncodingVersion& __v) const
    {
        return dynamic_cast<PcIdToWsServer*>(::IceProxy::Ice::Object::ice_encodingVersion(__v).get());
    }
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
    virtual ::IceProxy::Ice::Object* __newInstance() const;
};

}

}

namespace IceDelegate
{

namespace Pera
{

class News : virtual public ::IceDelegate::Ice::Object
{
public:

    virtual void NewMsg(const ::std::string&, const ::Ice::Context*, ::IceInternal::InvocationObserver&) = 0;
};

class PcIdToWsServer : virtual public ::IceDelegate::Ice::Object
{
public:

    virtual void TellClientId(const ::std::string&, const ::Ice::Context*, ::IceInternal::InvocationObserver&) = 0;
};

}

}

namespace IceDelegateM
{

namespace Pera
{

class News : virtual public ::IceDelegate::Pera::News,
             virtual public ::IceDelegateM::Ice::Object
{
public:

    virtual void NewMsg(const ::std::string&, const ::Ice::Context*, ::IceInternal::InvocationObserver&);
};

class PcIdToWsServer : virtual public ::IceDelegate::Pera::PcIdToWsServer,
                       virtual public ::IceDelegateM::Ice::Object
{
public:

    virtual void TellClientId(const ::std::string&, const ::Ice::Context*, ::IceInternal::InvocationObserver&);
};

}

}

namespace IceDelegateD
{

namespace Pera
{

class News : virtual public ::IceDelegate::Pera::News,
             virtual public ::IceDelegateD::Ice::Object
{
public:

    virtual void NewMsg(const ::std::string&, const ::Ice::Context*, ::IceInternal::InvocationObserver&);
};

class PcIdToWsServer : virtual public ::IceDelegate::Pera::PcIdToWsServer,
                       virtual public ::IceDelegateD::Ice::Object
{
public:

    virtual void TellClientId(const ::std::string&, const ::Ice::Context*, ::IceInternal::InvocationObserver&);
};

}

}

namespace Pera
{

class News : virtual public ::Ice::Object
{
public:

    typedef NewsPrx ProxyType;
    typedef NewsPtr PointerType;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual void NewMsg(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___NewMsg(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::Ice::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

protected:
    virtual void __writeImpl(::IceInternal::BasicStream*) const;
    virtual void __readImpl(::IceInternal::BasicStream*);
    #ifdef __SUNPRO_CC
    using ::Ice::Object::__writeImpl;
    using ::Ice::Object::__readImpl;
    #endif
};

inline bool operator==(const News& l, const News& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

inline bool operator<(const News& l, const News& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

class PcIdToWsServer : virtual public ::Ice::Object
{
public:

    typedef PcIdToWsServerPrx ProxyType;
    typedef PcIdToWsServerPtr PointerType;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual void TellClientId(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___TellClientId(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::Ice::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

protected:
    virtual void __writeImpl(::IceInternal::BasicStream*) const;
    virtual void __readImpl(::IceInternal::BasicStream*);
    #ifdef __SUNPRO_CC
    using ::Ice::Object::__writeImpl;
    using ::Ice::Object::__readImpl;
    #endif
};

inline bool operator==(const PcIdToWsServer& l, const PcIdToWsServer& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

inline bool operator<(const PcIdToWsServer& l, const PcIdToWsServer& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

}

namespace Pera
{

template<class T>
class CallbackNC_News_NewMsg : public Callback_News_NewMsg_Base, public ::IceInternal::OnewayCallbackNC<T>
{
public:

    typedef IceUtil::Handle<T> TPtr;

    typedef void (T::*Exception)(const ::Ice::Exception&);
    typedef void (T::*Sent)(bool);
    typedef void (T::*Response)();

    CallbackNC_News_NewMsg(const TPtr& obj, Response cb, Exception excb, Sent sentcb)
        : ::IceInternal::OnewayCallbackNC<T>(obj, cb, excb, sentcb)
    {
    }
};

template<class T> Callback_News_NewMsgPtr
newCallback_News_NewMsg(const IceUtil::Handle<T>& instance, void (T::*cb)(), void (T::*excb)(const ::Ice::Exception&), void (T::*sentcb)(bool) = 0)
{
    return new CallbackNC_News_NewMsg<T>(instance, cb, excb, sentcb);
}

template<class T> Callback_News_NewMsgPtr
newCallback_News_NewMsg(const IceUtil::Handle<T>& instance, void (T::*excb)(const ::Ice::Exception&), void (T::*sentcb)(bool) = 0)
{
    return new CallbackNC_News_NewMsg<T>(instance, 0, excb, sentcb);
}

template<class T> Callback_News_NewMsgPtr
newCallback_News_NewMsg(T* instance, void (T::*cb)(), void (T::*excb)(const ::Ice::Exception&), void (T::*sentcb)(bool) = 0)
{
    return new CallbackNC_News_NewMsg<T>(instance, cb, excb, sentcb);
}

template<class T> Callback_News_NewMsgPtr
newCallback_News_NewMsg(T* instance, void (T::*excb)(const ::Ice::Exception&), void (T::*sentcb)(bool) = 0)
{
    return new CallbackNC_News_NewMsg<T>(instance, 0, excb, sentcb);
}

template<class T, typename CT>
class Callback_News_NewMsg : public Callback_News_NewMsg_Base, public ::IceInternal::OnewayCallback<T, CT>
{
public:

    typedef IceUtil::Handle<T> TPtr;

    typedef void (T::*Exception)(const ::Ice::Exception& , const CT&);
    typedef void (T::*Sent)(bool , const CT&);
    typedef void (T::*Response)(const CT&);

    Callback_News_NewMsg(const TPtr& obj, Response cb, Exception excb, Sent sentcb)
        : ::IceInternal::OnewayCallback<T, CT>(obj, cb, excb, sentcb)
    {
    }
};

template<class T, typename CT> Callback_News_NewMsgPtr
newCallback_News_NewMsg(const IceUtil::Handle<T>& instance, void (T::*cb)(const CT&), void (T::*excb)(const ::Ice::Exception&, const CT&), void (T::*sentcb)(bool, const CT&) = 0)
{
    return new Callback_News_NewMsg<T, CT>(instance, cb, excb, sentcb);
}

template<class T, typename CT> Callback_News_NewMsgPtr
newCallback_News_NewMsg(const IceUtil::Handle<T>& instance, void (T::*excb)(const ::Ice::Exception&, const CT&), void (T::*sentcb)(bool, const CT&) = 0)
{
    return new Callback_News_NewMsg<T, CT>(instance, 0, excb, sentcb);
}

template<class T, typename CT> Callback_News_NewMsgPtr
newCallback_News_NewMsg(T* instance, void (T::*cb)(const CT&), void (T::*excb)(const ::Ice::Exception&, const CT&), void (T::*sentcb)(bool, const CT&) = 0)
{
    return new Callback_News_NewMsg<T, CT>(instance, cb, excb, sentcb);
}

template<class T, typename CT> Callback_News_NewMsgPtr
newCallback_News_NewMsg(T* instance, void (T::*excb)(const ::Ice::Exception&, const CT&), void (T::*sentcb)(bool, const CT&) = 0)
{
    return new Callback_News_NewMsg<T, CT>(instance, 0, excb, sentcb);
}

template<class T>
class CallbackNC_PcIdToWsServer_TellClientId : public Callback_PcIdToWsServer_TellClientId_Base, public ::IceInternal::OnewayCallbackNC<T>
{
public:

    typedef IceUtil::Handle<T> TPtr;

    typedef void (T::*Exception)(const ::Ice::Exception&);
    typedef void (T::*Sent)(bool);
    typedef void (T::*Response)();

    CallbackNC_PcIdToWsServer_TellClientId(const TPtr& obj, Response cb, Exception excb, Sent sentcb)
        : ::IceInternal::OnewayCallbackNC<T>(obj, cb, excb, sentcb)
    {
    }
};

template<class T> Callback_PcIdToWsServer_TellClientIdPtr
newCallback_PcIdToWsServer_TellClientId(const IceUtil::Handle<T>& instance, void (T::*cb)(), void (T::*excb)(const ::Ice::Exception&), void (T::*sentcb)(bool) = 0)
{
    return new CallbackNC_PcIdToWsServer_TellClientId<T>(instance, cb, excb, sentcb);
}

template<class T> Callback_PcIdToWsServer_TellClientIdPtr
newCallback_PcIdToWsServer_TellClientId(const IceUtil::Handle<T>& instance, void (T::*excb)(const ::Ice::Exception&), void (T::*sentcb)(bool) = 0)
{
    return new CallbackNC_PcIdToWsServer_TellClientId<T>(instance, 0, excb, sentcb);
}

template<class T> Callback_PcIdToWsServer_TellClientIdPtr
newCallback_PcIdToWsServer_TellClientId(T* instance, void (T::*cb)(), void (T::*excb)(const ::Ice::Exception&), void (T::*sentcb)(bool) = 0)
{
    return new CallbackNC_PcIdToWsServer_TellClientId<T>(instance, cb, excb, sentcb);
}

template<class T> Callback_PcIdToWsServer_TellClientIdPtr
newCallback_PcIdToWsServer_TellClientId(T* instance, void (T::*excb)(const ::Ice::Exception&), void (T::*sentcb)(bool) = 0)
{
    return new CallbackNC_PcIdToWsServer_TellClientId<T>(instance, 0, excb, sentcb);
}

template<class T, typename CT>
class Callback_PcIdToWsServer_TellClientId : public Callback_PcIdToWsServer_TellClientId_Base, public ::IceInternal::OnewayCallback<T, CT>
{
public:

    typedef IceUtil::Handle<T> TPtr;

    typedef void (T::*Exception)(const ::Ice::Exception& , const CT&);
    typedef void (T::*Sent)(bool , const CT&);
    typedef void (T::*Response)(const CT&);

    Callback_PcIdToWsServer_TellClientId(const TPtr& obj, Response cb, Exception excb, Sent sentcb)
        : ::IceInternal::OnewayCallback<T, CT>(obj, cb, excb, sentcb)
    {
    }
};

template<class T, typename CT> Callback_PcIdToWsServer_TellClientIdPtr
newCallback_PcIdToWsServer_TellClientId(const IceUtil::Handle<T>& instance, void (T::*cb)(const CT&), void (T::*excb)(const ::Ice::Exception&, const CT&), void (T::*sentcb)(bool, const CT&) = 0)
{
    return new Callback_PcIdToWsServer_TellClientId<T, CT>(instance, cb, excb, sentcb);
}

template<class T, typename CT> Callback_PcIdToWsServer_TellClientIdPtr
newCallback_PcIdToWsServer_TellClientId(const IceUtil::Handle<T>& instance, void (T::*excb)(const ::Ice::Exception&, const CT&), void (T::*sentcb)(bool, const CT&) = 0)
{
    return new Callback_PcIdToWsServer_TellClientId<T, CT>(instance, 0, excb, sentcb);
}

template<class T, typename CT> Callback_PcIdToWsServer_TellClientIdPtr
newCallback_PcIdToWsServer_TellClientId(T* instance, void (T::*cb)(const CT&), void (T::*excb)(const ::Ice::Exception&, const CT&), void (T::*sentcb)(bool, const CT&) = 0)
{
    return new Callback_PcIdToWsServer_TellClientId<T, CT>(instance, cb, excb, sentcb);
}

template<class T, typename CT> Callback_PcIdToWsServer_TellClientIdPtr
newCallback_PcIdToWsServer_TellClientId(T* instance, void (T::*excb)(const ::Ice::Exception&, const CT&), void (T::*sentcb)(bool, const CT&) = 0)
{
    return new Callback_PcIdToWsServer_TellClientId<T, CT>(instance, 0, excb, sentcb);
}

}

#endif
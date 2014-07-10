//
//  ProxyMacros.hpp
//  CYTOK
//
//  Created by Baptiste Dupy on 10/07/2014.
//
//

#ifndef CYTOK_ProxyMacros_hpp
#define CYTOK_ProxyMacros_hpp

#define DEFINE_PROXY(_BaseClass_,_ProxyClass_) \
static ck::proxy::ObjectProxy* createProxy()\
{ return new _ProxyClass_(); }\
static void registerObjectProxyFactory()\
{\
ck::proxy::ObjectProxyFactory::instance()\
->registerProduct(&_BaseClass_::type, \
ck::makeFunctor(&_ProxyClass_::createProxy));\
}


#define REGISTER_PROXY(_ProxyClass_) \
ck::proxy::ObjectProxyFactoryRegister<_ProxyClass_> registerProxy_##_ProxyClass_

#endif

//
//  ProxyMacros.hpp
//  CYTOK
//
//  Created by Baptiste Dupy on 10/07/2014.
//
//

#ifndef CYTOK_ProxyMacros_hpp
#define CYTOK_ProxyMacros_hpp

/////////////////////////////////////////////////////////////////////////////
///                 ObjectProxyFactory Macros
/////////////////////////////////////////////////////////////////////////////
/// <summary>
/// Defines a class as a proxy.
/// </summary>
/// This macro simply writes a static method createProxy
/// which returns an instance of the class and
/// a static method registerObjectProxyFactory
/// that need to be call from outside the class
/// using REGISTER_PROXY.
/// The method registerObjectProxyFactory
/// calls the singleton ObjectProxyFactory
/// and add the static function createProxy
/// as creator/functor for the factory
#define DEFINE_PROXY(_BaseClass_,_ProxyClass_) \
static ck::proxy::ObjectProxy* createProxy()\
{\
    return new _ProxyClass_();\
}\
static void registerObjectProxyFactory()\
{\
    ck::proxy::ObjectProxyFactoryMap::instance()\
    ->registerCreator(&_BaseClass_::type, \
    ck::makeFunctor(&_ProxyClass_::createProxy));\
}

/// <summary>
/// Register a proxy class to the factory.
/// </summary>
/// This macro simply creates an object of class
/// <c>ck::proxy::ObjectProxyFactoryRegister<_ProxyClass_></c>
/// Thus the default constructor of this object will be
/// called at runtime and will call the method
/// <c>_ProxyClass_::registerObjectProxyFactory()</c>
/// This is then necessary to define the class <c>_ProxyClass_</c>
/// as a proxy class by using DEFINE_PROXY.
#define REGISTER_PROXY(_ProxyClass_) \
ck::proxy::ObjectProxyFactoryRegister<_ProxyClass_> registerProxy_##_ProxyClass_



/////////////////////////////////////////////////////////////////////////////
///                 ObjectPropertyFactory Macros
/////////////////////////////////////////////////////////////////////////////

#define DEFINE_PROPERTY(_PropertyType_,_Class_) \
static ck::proxy::ObjectProperty* createProperty()\
{\
    return new _Class_();\
} \
static void registerObjectPropertyFactory()\
{\
    ck::proxy::ObjectPropertyFactoryMap::instance()\
    ->registerCreator(_PropertyType_,\
    ck::makeFunctor(&_Class_::createProperty));\
}

#define REGISTER_PROPERTY(_Class_) \
ck::proxy::ObjectPropertyFactoryRegister<_Class_> registerProperty_##_Class_


#endif

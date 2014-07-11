//
//  ObjectProxyFactory_impl.hpp
//  CYTOK
//
//  Created by Baptiste Dupy on 11/07/2014.
//
//

#ifndef CYTOK_ObjectProxyFactory_impl_hpp
#define CYTOK_ObjectProxyFactory_impl_hpp

#include "Cytok/proxy/ObjectProxy.hpp"

namespace ck
{
	namespace proxy
    {
        template<typename BaseClass>
        ObjectProxy* ObjectProxyFactory::createProxy()
        {
            return createProduct(&BaseClass::type);
        }
        
        template<typename BaseClass>
        ObjectProxy* ObjectProxyFactory::createProxy(BaseClass* obj)
        {
            if(!obj)
                return createProxy<BaseClass>();
            
            ObjectProxy* proxy = createProduct(obj->classType());
            
            if(proxy)
                proxy->setObject(obj);
            
            return proxy;
        }
        
    }
}

#endif

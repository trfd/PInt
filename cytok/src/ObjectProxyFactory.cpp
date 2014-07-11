//
//  ObjectProxyFactory.cpp
//  CYTOK
//
//  Created by Baptiste Dupy on 10/07/2014.
//
//

#include "Cytok/Object.hpp"

#include "Cytok/proxy/ObjectProxyFactory.hpp"

namespace ck
{
    namespace proxy
    {
        ObjectProxyFactory* ObjectProxyFactory::myCurrFactory = NULL;
        
        ObjectProxyFactory* ObjectProxyFactory::currentFactory()
        {
            return myCurrFactory;
        }
        
        void ObjectProxyFactory::setCurrentFactory(ObjectProxyFactory* f)
        {
            myCurrFactory = f;
        }
        
        ObjectProxyFactory::ObjectProxyFactory()
        : Factory(ObjectProxyFactoryMap::instance())
        {}
        

    }
}
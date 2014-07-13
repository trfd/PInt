//
//  ObjectProxyFactory.hpp
//  CYTOK
//
//  Created by Baptiste Dupy on 10/07/2014.
//
//

#ifndef CYTOK_ObjectProxyFactory_hpp
#define CYTOK_ObjectProxyFactory_hpp

#include <map>

#include "Cytok/ClassType.hpp"
#include "Cytok/Functor.hpp"
#include "Cytok/utils/Singleton.hpp"
#include "Cytok/Factory.hpp"
#include "Cytok/proxy/ObjectProxyFactoryError.hpp"

namespace ck
{
    namespace proxy
    {
        class ObjectProxy;
        
        class ObjectProxyFactoryMap
        : public FactoryMap
        <
            ObjectProxy,
            ClassType* ,
            Functor<ObjectProxy*> ,
            ObjectProxyFactoryError
        >,
        public utils::Singleton<ObjectProxyFactoryMap>
        {
        public:
        };
        
        class ObjectProxyFactory
        : public Factory
        <
            ObjectProxy,
            ClassType*,
            Functor<ObjectProxy*>,
            ObjectProxyFactoryError
        >
        {
            
        public:
            
            static ObjectProxyFactory* currentFactory();
            
            static void setCurrentFactory(ObjectProxyFactory* f);
            
            ObjectProxyFactory();
            
            template<typename BaseClass>
            ObjectProxy* createProxy();
            
            template<typename BaseClass>
            ObjectProxy* createProxy(BaseClass* obj);
            
        private:
            
            static ObjectProxyFactory* myCurrFactory;
            
        };
        
        template<typename Type>
        struct ObjectProxyFactoryRegister
        {
            ObjectProxyFactoryRegister()
            {
                Type::registerObjectProxyFactory();
            }
        };

    }
}

#include "ObjectProxyFactory_impl.hpp"

#endif

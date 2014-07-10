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
#include "Cytok/proxy/ObjectProxy.hpp"

namespace ck
{
    namespace proxy
    {
        class ObjectProxy;
        
        template<typename ProductID>
        class ObjectProxyFactoryError
        {
            static void OnUnknownType(const ProductID& id)
            {
                FactoryException<ProductID>::OnUnknownType(id);
            }
            static void OnNullProduct(const ProductID& id)
            {
                FactoryException<ProductID>::OnNullProduct(id);
            }
        };
        
        
        template<>
        class ObjectProxyFactoryError<ClassType*>
        {
            ClassType* myId;
            
        public:
            
            ObjectProxyFactoryError(ClassType* id)
            {
                myId = id;
            }
            
            virtual const char* what() const throw()
            {
                if(myId == NULL)
                    return "NULL ID passed to ObjectProxyFactory\n";
                
                return (std::string("Unknow ID passed to factory: ")+myId->fullname()).c_str();
            }
            
            ClassType* id()
            {
                return myId;
            }
            
            static void OnUnknownType(ClassType* id)
            {
                throw ObjectProxyFactoryError(id);
            }
            static void OnNullProduct(ClassType* id)
            {
                throw ObjectProxyFactoryError(id);
            }
        };
    
        
        class ObjectProxyFactory
        : public Factory<
            ObjectProxy,
            ClassType* ,
            Functor<ObjectProxy*> ,
            ObjectProxyFactoryError
                > ,
          public utils::Singleton<ObjectProxyFactory>
        {
        public:
            
            template<typename BaseClass>
            ObjectProxy* createProxy()
            {
                return createProduct(&BaseClass::type);
            }
            
            template<typename BaseClass>
            ObjectProxy* createProxy(BaseClass* obj)
            {
                if(!obj)
                    return createProxy<BaseClass>();
                
                ObjectProxy* proxy = createProduct(obj->classType());
                
                if(proxy)
                    proxy->setObject(obj);
                
                return proxy;
            }
            
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

#endif

//
//  ObjectProxyFactoryError.hpp
//  CYTOK
//
//  Created by Baptiste Dupy on 11/07/2014.
//
//

#ifndef CYTOK_ObjectProxyFactoryError_hpp
#define CYTOK_ObjectProxyFactoryError_hpp

namespace ck
{
	namespace proxy
    {
        template<typename ProductID>
        class ObjectProxyFactoryError
        {
            static void OnUnknownType(const ProductID& id)
            {
                FactoryError<ProductID>::OnUnknownType(id);
            }
            
            static void OnNullProduct(const ProductID& id)
            {
                FactoryError<ProductID>::OnNullProduct(id);
            }
            
            static void OnNullMap()
            {
                FactoryError<ProductID>::OnNullMap();
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
            
            static void OnNullMap()
            {
                throw FactoryNullMapException();
            }
        };

    }
}

#endif

//
//  ObjectProxy_impl.hpp
//  CYTOK
//
//  Created by Baptiste Dupy on 11/07/2014.
//
//

#ifndef CYTOK_ObjectProxy_impl_hpp
#define CYTOK_ObjectProxy_impl_hpp

#include "Cytok/proxy/ObjectPropertyGeneric.hpp"
#include "Cytok/proxy/ObjectProxyProperty.hpp"
#include "Cytok/proxy/ObjectPropertyFactory.hpp"
#include "Cytok/proxy/ObjectProxyFactory.hpp"


namespace ck
{
	namespace proxy
    {
        template<
            typename BaseClass ,
            typename GetType,
            typename SetType
        >
        void ObjectProxy::addDefaultProperty(ObjectProperty::PropertyType type,
                                             const std::string& name,
                                             BaseClass* ptr ,
                                             GetType(BaseClass::*getter)(),
                                             void(BaseClass::*setter)(SetType))
        {
            ObjectProperty* prop = ObjectPropertyFactory::currentFactory()->createProduct(type);
            
            prop->setName(name);
            
            ObjectPropertyGeneric<GetType,SetType>* genericProp =
                dynamic_cast<ObjectPropertyGeneric<GetType,SetType>*>(prop);
            
            if(genericProp)
            {
                genericProp->setGetter(ptr, getter);
                genericProp->setSetter(ptr, setter);
            }
            else
                throw InvalidPropertyException();
            
            this->addProperty(genericProp);
        }
        
        template<
            typename CustomPropertyType,
            typename BaseClass ,
            typename GetType,
            typename SetType
        >
        void ObjectProxy::addCustomProperty(const std::string& name,
                                            BaseClass* ptr ,
                                            GetType(BaseClass::*getter)(),
                                            void(BaseClass::*setter)(SetType))
        {
            CustomPropertyType* prop = new CustomPropertyType();
            
            prop->setName(name);
            
            ObjectPropertyGeneric<GetType,SetType>* genericProp =
                dynamic_cast<ObjectPropertyGeneric<GetType,SetType>*>(prop);
            
            if(genericProp)
            {
                genericProp->setGetter(getter);
                genericProp->setSetter(setter);
            }
            else
                throw InvalidPropertyException();
            
            this->addProperty(prop);
        }
        
        template<typename BaseClass>
        void ObjectProxy::addProxyProperty(const std::string& name, BaseClass* obj )
        {
            this->addProperty(new ObjectProxyProperty(name , ObjectProxyFactory::currentFactory()->createProxy(obj)));
        }
    }
}

#endif

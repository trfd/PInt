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
        
        /// RequiredBase is the base class
        /// from which the property object must derive
        /// example: in the basic implementation,
        /// ck::proxy::ObjectProperty::INTEGER is
        /// associated to an ObjectPropertGeneric<int,int> subclass
        /// (namely IntPropery) thus the call for adding a property
        /// of type in would be:
        /// addPropperty<ObjectPropertGeneric<int,int>>(
        ///     ck::proxy::ObjectProperty::INTEGER,
        ///     "Parameter Lambda", ... )
        template
        <
            typename RequiredBase,
            typename BaseClass,
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
            
            RequiredBase* baseProp =
                dynamic_cast<RequiredBase*>(prop);
            
            if(baseProp)
                baseProp->setAccessors(ptr, getter , setter);
            else
                throw InvalidPropertyException();
            
            this->addProperty(baseProp);
        }
        
        template
        <
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

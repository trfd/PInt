//
//  ObjectProxyProperty.cpp
//  CYTOK
//
//  Created by Baptiste Dupy on 11/07/2014.
//
//

#include "Cytok/proxy/ObjectProxy.hpp"
#include "Cytok/proxy/ObjectPropertyFactory.hpp"
#include "Cytok/proxy/ObjectProxyProperty.hpp"

namespace ck
{
    namespace proxy
    {
        
        REGISTER_PROPERTY(ObjectProxyProperty);
    
        ObjectProxyProperty::ObjectProxyProperty()
        {}
        
        ObjectProxyProperty::ObjectProxyProperty(const std::string& name, ObjectProxy* proxy)
        : ObjectProperty(name)
        {
            myProxy = proxy;
        }
        
        ObjectProxy* ObjectProxyProperty::value()
        {
            return myProxy;
        }
        
        void ObjectProxyProperty::setValue(ObjectProxy* value)
        {
            if(myProxy)
                delete myProxy;
            
            myProxy = value;
        }
        
        std::string ObjectProxyProperty::valueToString()
        {
            return myProxy->name();
        }
        
        void ObjectProxyProperty::valueFromString(const std::string& str)
        {
            myProxy->setName(str);
        }
        
        size_t ObjectProxyProperty::propertyCount()
        {
            return myProxy->propertyCount();
        }
        
        IObjectPropertyHolder::PropertyList* ObjectProxyProperty::properties()
        {
            return myProxy->properties();
        }
        
        ObjectProperty* ObjectProxyProperty::property(int atIdx)
        {
            return myProxy->property(atIdx);
        }
        
        ObjectProperty* ObjectProxyProperty::property(std::string pName)
        {
            return myProxy->property(pName);
        }
    }
}
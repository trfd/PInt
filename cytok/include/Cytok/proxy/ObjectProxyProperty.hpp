//
//  ObjectProxyProperty.hpp
//  CYTOK
//
//  Created by Baptiste Dupy on 10/07/2014.
//
//

#ifndef CYTOK_ObjectProxyProperty_hpp
#define CYTOK_ObjectProxyProperty_hpp

#include "Cytok/Logs.hpp"

#include "Cytok/proxy/ObjectProxy.hpp"
#include "Cytok/proxy/ObjectPropertyGeneric.hpp"

namespace ck
{
    namespace proxy
    {
        class ObjectProxy;
        
        class ObjectProxyProperty : public ObjectProperty
        {
		public:
            
            DEFINE_PROPERTY(ObjectProperty::OBJECT, ObjectProxyProperty)
            
            ObjectProxyProperty();
            
            ObjectProxyProperty(const std::string& name, ObjectProxy* proxy);
            
            virtual ObjectProxy* value();
            
            virtual void setValue(ObjectProxy* value);
            
            virtual std::string valueToString();
            
            virtual void valueFromString(const std::string& str);
            
            virtual size_t propertyCount();
            
            virtual PropertyList* properties();
            
            virtual ObjectProperty* property(int atIdx);
            
            virtual ObjectProperty* property(std::string pName);
            
        private:
            
            ObjectProxy* myProxy = nullptr;
        };
    }
}

#endif

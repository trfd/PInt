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

#include "Cytok/proxy/ObjectPropertyGeneric.hpp"

namespace ck
{
    namespace proxy
    {
        class ObjectProxy;
        
        class ObjectProxyProperty : public ObjectProperty
        {
		public:
            
            ObjectProxyProperty(const std::string& name, ObjectProxy* proxy)
            : ObjectProperty(name)
            {
                myProxy = proxy;
                myProxy->buildProxy();
            }
            
            inline virtual ObjectProxy* value()
            {
                return myProxy;
            }
            
            inline virtual void setValue(ObjectProxy* value)
            {
                if(myProxy)
                    delete myProxy;
                    
                myProxy = value;
            }
            
            inline virtual std::string valueToString()
            {
                return myProxy->name();
            }
            
            inline virtual void valueFromString(const std::string& str)
            {
                myProxy->setName(str);
            }
            
            inline virtual size_t propertyCount()
            {
                return myProxy->propertyCount();
            }
            
            inline virtual PropertyList* properties()
            {
                return myProxy->properties();
            }
            
            inline virtual ObjectProperty* property(int atIdx)
            {
                return myProxy->property(atIdx);
            }
            
            inline virtual ObjectProperty* property(std::string pName)
            {
                return myProxy->property(pName);
            }
            
        private:
            
            ObjectProxy* myProxy = nullptr;
        };
    }
}

#endif

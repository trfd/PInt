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
        
        class ObjectProxyProperty : public ObjectPropertyGeneric<ObjectProxy*>
        {
		public:
            
            template<typename ProxyType,typename... Args>
            ObjectProxyProperty(const std::string& name,
                                Args&&... args)
            {
                if(!std::is_base_of<ck::proxy::ObjectProxy,ProxyType>::value)
                {
                    LOG_ERROR_I(Log::ENGINE)<<"ObjectProxyProperty's type must derive "
                    <<"from ck::proxy::ObjectProxy"<<LOG_END;
                }
                
                myProxy = new ProxyType(std::forward<Args>(args)...);
            }
            
            inline virtual ObjectProxy* value()
            {
                return myProxy;
            }
            
            inline virtual void setValue(ObjectProxy*&& value)
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

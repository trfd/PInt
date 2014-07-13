//
//  ObjectProxy.hpp
//  CYTOK
//
//  Created by Baptiste Dupy on 09/07/2014.
//
//

#ifndef CYTOK_ObjectProxy_hpp
#define CYTOK_ObjectProxy_hpp

#include <string>
#include <vector>

#include "Cytok/Object.hpp"
#include "Cytok/proxy/ObjectProperty.hpp"
#include "ObjectPropertyHolder.hpp"

namespace ck
{
    namespace proxy
    {
        
        class ObjectProxy : public IObjectPropertyHolder
        {
        public:
            
            ObjectProxy()
            {
                myObject = NULL;
            }
            
            ObjectProxy(ck::Object* obj)
            {
                myObject = obj;
            }
            
            void setObject(ck::Object* obj)
            {
                this->clearProxy();
                
                myObject = obj;

                if(myObject)
                    this->buildProxy();
            }
            
            void setName(const std::string& name);
            
            std::string& name();
            
            virtual size_t propertyCount();
            
            virtual PropertyList* properties();
            
            virtual ObjectProperty* property(int atIdx);
            
            virtual ObjectProperty* property(std::string pName);
            
            /// Use the current ObjectPropertyFactory
            template
            <
            typename RequiredBase,
            typename BaseClass,
            typename GetType,
            typename SetType
            >
            void addDefaultProperty(ObjectProperty::PropertyType type,
                                    const std::string& name,
                                    BaseClass* ptr ,
                                    GetType(BaseClass::*getter)(),
                                    void(BaseClass::*setter)(SetType));

            
            template<
                typename CustomPropertyType,
                typename BaseClass ,
                typename GetType,
                typename SetType
                >
            void addCustomProperty(const std::string& name,
                                   BaseClass* ptr ,
                                   GetType(BaseClass::*getter)(),
                                   void(BaseClass::*setter)(SetType));
            
            template<typename BaseClass>
            void addProxyProperty(const std::string& name, BaseClass* obj );
            
            virtual void addProperty(ObjectProperty* prop);
            
            ck::Object* object();
            
            virtual void buildProxy();
            
            virtual void clearProxy();
            
            virtual ~ObjectProxy(){};
            
        private:
            
            std::string myName;
            
            PropertyList myProperties;
            
            ck::Object* myObject;

        };
    }
}

#include "ObjectProxy_impl.hpp"

#endif

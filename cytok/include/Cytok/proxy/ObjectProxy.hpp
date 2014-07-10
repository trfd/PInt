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

#include "ObjectPropertyHolder.hpp"

namespace ck
{
    namespace proxy
    {
        class ObjectProperty;
        
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
                myObject = obj;

                if(myObject)
                    this->buildProxy();
            }
            
            void setName(const std::string& name);
            
            std::string& name();
            
            void addProperty(ObjectProperty* prop);
            
            virtual size_t propertyCount();
            
            virtual PropertyList* properties();
            
            virtual ObjectProperty* property(int atIdx);
            
            virtual ObjectProperty* property(std::string pName);

            
            ck::Object* object();
            
            virtual void buildProxy();
            
            virtual ~ObjectProxy(){};
            
        private:
            
            std::string myName;
            
            PropertyList myProperties;
            
            ck::Object* myObject;

        };
    }
}

#endif

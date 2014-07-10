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

namespace ck
{
    namespace proxy
    {
        class ObjectProperty;
        
        class ObjectProxy 
        {
        public:
            
            typedef std::vector<ObjectProperty*> PropertyList;
            typedef std::vector<ObjectProperty*>::iterator PropertyList_it;
            
            typedef std::vector<ObjectProxy*> ProxyList;
            typedef std::vector<ObjectProxy*>::iterator ProxyList_it;
            
            ObjectProxy(ck::Object* obj)
            {
                myObject = obj;
            }
            
            void setName(const std::string& name);
            
            std::string& name();
            
            void addProperty(ObjectProperty* prop);
            
            PropertyList& properties();
            
            ObjectProperty* property(int atIdx);
            
            ObjectProperty* property(std::string pName);
            
            
            void addProxy(ObjectProxy* prox);
            
            ProxyList& proxies();
            
            ObjectProxy* proxy(int atIdx);
            
            ObjectProxy* proxy(std::string pName);
            
            ck::Object* object();
            
            virtual void buildProxy();
            
            virtual ~ObjectProxy(){};
            
        private:
            
            std::string myName;
            
            PropertyList myProperties;
            
            ProxyList myProxies;
            
            ck::Object* myObject;

        };
    }
}

#endif

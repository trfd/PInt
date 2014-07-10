//
//  ObjectProxy.cpp
//  CYTOK
//
//  Created by Baptiste Dupy on 09/07/2014.
//
//

#include "Cytok/proxy/ObjectProperty.hpp"
#include "Cytok/proxy/ObjectProxy.hpp"

namespace ck
{
    namespace proxy
    {
        void ObjectProxy::setName(const std::string& name)
        {
            myName = name;
        }
        
        std::string& ObjectProxy::name()
        {
            return myName;
        }
        
        void ObjectProxy::addProperty(ObjectProperty* prop)
        {
            if(prop == NULL)
                return;
            
            myProperties.push_back(prop);
        }
        
        std::vector<ObjectProperty*>& ObjectProxy::properties()
        {
            return myProperties;
        }
        
        ObjectProperty* ObjectProxy::property(int atIdx)
        {
            if(atIdx < 0 || atIdx >= myProperties.size() )
                return NULL;
            
            return myProperties[atIdx];
        }
        
        ObjectProperty* ObjectProxy::property(std::string pName)
        {
            PropertyList_it it = std::find_if(myProperties.begin(), myProperties.end(),
                                              [&](ObjectProperty* p) -> bool
            {
                return  p->name() == pName;
            });
            
            if(it == myProperties.end())
                return NULL;
            
            return *it;
        }
        
        
        void ObjectProxy::addProxy(ObjectProxy* prox)
        {
            myProxies.push_back(prox);
        }
        
        std::vector<ObjectProxy*>& ObjectProxy::proxies()
        {
            return myProxies;
        }
        
        ObjectProxy* ObjectProxy::proxy(int atIdx)
        {
            return myProxies[atIdx];
        }
        
        ObjectProxy* ObjectProxy::proxy(std::string pName)
        {
            ProxyList_it it = std::find_if(myProxies.begin(), myProxies.end(),
                                           [&](ObjectProxy* p) -> bool
            {
                return  p->name() == pName;
            });
            
            if(it == myProxies.end())
                return NULL;
            
            return *it;
        }
        
        ck::Object* ObjectProxy::object()
        {
            return myObject;
        }
        
        void ObjectProxy::buildProxy()
        {}
    }
}
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
            
            this->myProperties.push_back(prop);
        }
        
                
        ck::Object* ObjectProxy::object()
        {
            return myObject;
        }
        
        void ObjectProxy::buildProxy()
        {}
        
        /// IObjectPropertyHolder
        
        size_t ObjectProxy::propertyCount()
        {
            return myProperties.size();
        }
        
        ObjectProxy::PropertyList* ObjectProxy::properties()
        {
            return &myProperties;
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
    }
}
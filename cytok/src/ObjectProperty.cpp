//
//  ObjectProperty.cpp
//  CYTOK
//
//  Created by Baptiste Dupy on 09/07/2014.
//
//

#include "Cytok/proxy/ObjectProperty.hpp"

namespace ck
{
    namespace proxy
    {
        ObjectProperty::ObjectProperty()
        {
        }
        
        ObjectProperty::ObjectProperty(const std::string& name)
        : myName(name)
        {}
        
        std::string& ObjectProperty::name()
        {
            return myName;
        }
        
        void ObjectProperty::setName(const std::string &name)
        {
            myName = name;
        }
        
        ObjectProperty::PropertyType ObjectProperty::propertyType()
        {
            return ObjectProperty::property_type;
        }
        
        size_t ObjectProperty::propertyCount()
        {
            return 0;
        }
        
        ObjectProperty::PropertyList* ObjectProperty::properties()
        {
            return NULL;
        }
        
        ObjectProperty* ObjectProperty::property(int atIdx)
        {
            return NULL;
        }
        
        ObjectProperty* ObjectProperty::property(std::string pName)
        {
            return NULL;
        }

    }
}
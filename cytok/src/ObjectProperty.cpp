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
        ObjectProperty::ObjectProperty(const std::string& name ,
                                       ObjectProperty::PropertyType type)
        : myName(name) , myType(type)
        {}
        
        std::string& ObjectProperty::name()
        {
            return myName;
        }
        
        ObjectProperty::PropertyType ObjectProperty::type()
        {
            return myType;
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
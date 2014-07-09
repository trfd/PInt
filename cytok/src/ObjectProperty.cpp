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
        ObjectProperty::ObjectProperty(const std::string& name)
        : myName(name)
        {}
        
        std::string& ObjectProperty::name()
        {
            return myName;
        }
    }
}